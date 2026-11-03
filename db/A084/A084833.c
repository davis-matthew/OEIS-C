/*
 * A084833  a(n) is the smallest number such that a(n) + a(n-1), a(n) + a(n-1)
 *          + a(n-2), ..., a(n) + ... + a(1) are nonprime.
 * https://oeis.org/A084833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-03)
 * Helpers:   is_prime_seq, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A084833:
   a(n) is the smallest positive integer such that all sums
   a(n)+a(n-1)+…+a(n-k) for 1 ≤ k ≤ n-1 are composite.
   The algorithm builds the sequence term by term, retaining a
   running prefix sum array to test candidate values efficiently. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Primality tests for 64‑bit and arbitrary‑precision integers. */
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static seq_t *a_vals          = NULL;   /* a(1) … a(computed)         */
static uint64_t *pref64       = NULL;   /* pref[0]=0, pref[i]=sum a(1) … a(i) for 8‑bit case */
static seq_t *pref_seq        = NULL;   /* same but for large seq_t      */
static size_t cap_a           = 0;     /* capacity of a_vals           */
static size_t cap_pref64      = 0;     /* capacity of pref64           */
static size_t cap_pref_seq    = 0;     /* capacity of pref_seq          */
static idx_t  computed        = 0;     /* number of terms already computed */

/* Ensure that terms up to index n (≥1) are computed. */
static void ensure(idx_t n)
{
    if (computed >= n) return;

    /* Initial allocation */
    if (!a_vals) {
        size_t init = 16;
        a_vals = mem_new(seq_t, init);
        cap_a  = init;

        if (sizeof(seq_t) <= 8) {                 /* 8‑bit and smaller */
            pref64 = (uint64_t*)malloc((init + 1) * sizeof(uint64_t));
            pref64[0] = 0ull;
            cap_pref64 = init + 1;
        } else {                                  /* larger types */
            pref_seq = mem_new(seq_t, (init + 1));
            pref_seq[0] = 0;
            cap_pref_seq = init + 1;
        }
    }

    /* Compute missing terms */
    while (computed < n) {
        /* Grow a_vals if necessary */
        if (computed >= cap_a) {
            size_t new_cap = cap_a * 2;
            a_vals = mem_resize(seq_t, a_vals, new_cap);
            cap_a = new_cap;
        }

        /* Grow the appropriate prefix array */
        if (sizeof(seq_t) <= 8) {
            if (computed + 1 >= cap_pref64) {
                size_t new_cap = cap_pref64 * 2;
                pref64 = (uint64_t*)realloc(pref64, new_cap * sizeof(uint64_t));
                cap_pref64 = new_cap;
            }
        } else {
            if (computed + 1 >= cap_pref_seq) {
                size_t new_cap = cap_pref_seq * 2;
                pref_seq = mem_resize(seq_t, pref_seq, new_cap);
                cap_pref_seq = new_cap;
            }
        }

        /* Find smallest admissible candidate */
        seq_t cand = 1;
        if (sizeof(seq_t) <= 8) {
            uint64_t sum_prev = pref64[computed];   /* sum of a(1)…a(computed) */
            for (;;) {
                bool ok = true;
                for (idx_t j = 0; j < computed; ++j) {
                    uint64_t r = (uint64_t)cand + (sum_prev - pref64[j]);
                    if (is_prime_seq(r)) { ok = false; break; }
                }
                if (ok) break;
                ++cand;   /* increment until all sums are composite */
            }
            a_vals[computed] = cand;
            pref64[computed + 1] = sum_prev + (uint64_t)cand;
        } else {
            seq_t sum_prev = pref_seq[computed];
            for (;;) {
                bool ok = true;
                for (idx_t j = 0; j < computed; ++j) {
                    seq_t r = cand + (sum_prev - pref_seq[j]);
                    if (is_prime_big(r)) { ok = false; break; }
                }
                if (ok) break;
                ++cand;
            }
            a_vals[computed] = cand;
            pref_seq[computed + 1] = sum_prev + cand;
        }

        ++computed;
    }
}

/* Public API: return the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 – guard */
    ensure(n);
    return a_vals[n - 1];          /* array is zero‑based */
}
