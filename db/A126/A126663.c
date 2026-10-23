/*
 * A126663  Absolute difference between largest prime factors of two
 *          successive semiprimes.
 * https://oeis.org/A126663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126663: absolute difference between the largest prime factors of successive semiprimes */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------
   Internal caches
-------------------------------------------------------------------- */
static seq_t *diffs = NULL;    /* sequence values a(1), a(2), …          */
static size_t diffs_len = 0;   /* number of values currently stored     */
static size_t diffs_cap = 0;   /* allocated capacity of diffs array      */
static uint64_t last_checked = 0;   /* last integer tested for semiprimality */
static uint64_t prev_lpf = 0;  /* largest prime factor of previous semiprime */
static bool first_semiprime_found = false;

/* --------------------------------------------------------------------
   Small helper functions for 64‑bit arithmetic
-------------------------------------------------------------------- */
static inline uint64_t smallest_prime_factor(uint64_t n)
{
    if (n <= 1) return n;
    if (n % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return d;
    return n;   /* n is prime */
}

static inline uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t lp = 0, x = n;
    while (x % 2 == 0) { lp = 2; x /= 2; }
    for (uint64_t d = 3; d * d <= x; d += 2)
        while (x % d == 0) { lp = d; x /= d; }
    if (x > 1) lp = x;
    return lp;
}

static inline bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;
    uint64_t p = smallest_prime_factor(n);
    if (p == n) return false;             /* prime */
    uint64_t q = n / p;
    return q > 1 && q == smallest_prime_factor(q);   /* q prime */
}

/* --------------------------------------------------------------------
   Grow the cache so that it contains at least `target` values a(1)..a(target)
-------------------------------------------------------------------- */
static void extend_cache(size_t target)
{
    if (diffs_len >= target) return;

    /* Allocate / grow the output array lazily */
    if (diffs_cap == 0) {
        diffs_cap = 16;
        diffs = (seq_t *)malloc(diffs_cap * sizeof(seq_t));
        if (!diffs) abort();
    }

    while (diffs_len < target) {
        /* Find next semiprime */
        while (!is_semiprime(++last_checked))
            ;   /* spin */

        uint64_t lpf = largest_prime_factor(last_checked);

        if (!first_semiprime_found) {
            prev_lpf = lpf;
            first_semiprime_found = true;
        } else {
            /* Compute the absolute difference of the two LPFs */
            uint64_t diff = (lpf >= prev_lpf) ? (lpf - prev_lpf) : (prev_lpf - lpf);
            seq_t val = (seq_t)diff;

            /* Grow the output array if necessary */
            if (diffs_len == diffs_cap) {
                size_t new_cap = diffs_cap * 2;
                seq_t *tmp = (seq_t *)realloc(diffs, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                diffs = tmp;
                diffs_cap = new_cap;
            }
            diffs[diffs_len++] = val;
            prev_lpf = lpf;
        }
    }
}

/* --------------------------------------------------------------------
   Public entry point
-------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined below offset 1 */
    extend_cache((size_t)n);           /* make sure a(1)..a(n) are computed */
    return diffs[n - 1];               /* zero‑based array indexing */
}
