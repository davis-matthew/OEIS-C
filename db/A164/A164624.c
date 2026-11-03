/*
 * A164624  Primes p such that p + Floor[p/2] + Floor[p/3] is prime.
 * https://oeis.org/A164624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan primes p and keep those for which
   p + ⌊p/2⌋ + ⌊p/3⌋ is also prime.  The sequence is memoised
   across calls. */

#include <stdlib.h>
#include <stdbool.h>
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

static seq_t *good_cache = NULL;
static size_t good_len   = 0; /* number of entries stored */
static size_t good_cap   = 0; /* allocated capacity */

/* Last candidate examined (unsigned 64‑bit). */
static idx_t last_checked = 1;

/* Compute maximum value representable in seq_t, converted to idx_t. */
static const idx_t MAX_SEQ_T = (idx_t)(-(seq_t)1);

/* Ensure buffer can hold at least 'need' elements. */
static void
ensure_good_capacity(size_t need)
{
    if (need <= good_cap)
        return;
    size_t new_cap = good_cap ? good_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    /* In C++ a cast is required. */
    good_cache = mem_resize(seq_t, good_cache, new_cap);
    good_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset 1: a(1) is the first prime. */
        return 0;

    /* Generate terms until we have at least n. */
    while (good_len < (size_t)n)
    {
        /* Find the next prime satisfying the condition. */
        while (1)
        {
            const idx_t cand = ++last_checked;

            /* Stop if candidate cannot be stored in seq_t. */
            if (cand > MAX_SEQ_T)
                return 0;

            if (!is_prime_big((seq_t)cand))
                continue;

            /* Compute q = p + floor(p/2) + floor(p/3) in a larger type to
               avoid overflow before converting back to seq_t. */
            const idx_t q_idx = cand + cand / 2 + cand / 3;

            if (q_idx > MAX_SEQ_T)     /* q would overflow seq_t */
                continue;

            const seq_t q = (seq_t)q_idx;

            if (is_prime_big(q))
            {
                ensure_good_capacity(good_len + 1);
                good_cache[good_len++] = (seq_t)cand;
                break;   /* None the next sequence member. */
            }
        }
    }

    return good_cache[n - 1];
}
