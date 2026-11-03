/*
 * A126769  Primes p of the form k^4 + s where k > 1, s >= 1 and k^2 + s is
 *          also prime.
 * https://oeis.org/A126769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 5274, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9122 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126769: primes p such that p = k^4 + s (k>1, s≥1)
   and k^2 + s is also prime.  The i‑th term a(i) is the i‑th such prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

/* ---------------------------------------------------------------------- */
/*  Static storage for the computed sequence                              */
/* ---------------------------------------------------------------------- */
static seq_t *seq_arr = NULL;   /* dynamic array to store the terms      */
static size_t seq_len = 0;      /* current number of terms stored        */
static size_t seq_cap = 0;      /* capacity of seq_arr                    */

static uint64_t next_candidate = 17;  /* next integer to test (primes ≥ 17)    */

/* ---------------------------------------------------------------------- */
/*  Ensure that at least n terms have been generated.                     */
/* ---------------------------------------------------------------------- */
static void ensure_sequence(idx_t n)
{
    if (seq_len >= n)
        return;

    uint64_t cand = next_candidate;

    while (seq_len < n) {
        seq_t cand_val = (seq_t)cand;

        if (is_prime_big(cand_val)) {             /* check primality of p   */
            bool ok = false;

            /* Try all possible k > 1 with k^4 < p */
            for (int k = 2; ; ++k) {
                seq_t ksq = (seq_t)k * k;          /* k^2                  */
                seq_t k4  = ksq * ksq;             /* k^4                  */

                if (k4 >= cand_val)                /* no larger k possible */
                    break;

                seq_t s = cand_val - k4;           /* s = p - k^4           */
                seq_t q = ksq + s;                 /* q = k^2 + s           */

                if (is_prime_big(q)) {             /* check q primality     */
                    ok = true;
                    break;
                }
            }

            if (ok) {                            /* store the term */
                if (seq_len == seq_cap) {
                    seq_cap = seq_cap ? seq_cap * 2 : 16;
                    seq_arr = mem_resize(seq_t, seq_arr, seq_cap);
                }
                seq_arr[seq_len++] = cand_val;
            }
        }
        ++cand;
    }
    next_candidate = cand;                       /* remember where we stopped */
}

/* ---------------------------------------------------------------------- */
/*  Return the n‑th term of the sequence                                 */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for index 0, but harmless */

    ensure_sequence(n);
    return seq_arr[n - 1];
}
