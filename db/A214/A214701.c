/*
 * A214701  Number of different values taken by sigma(k)/k for k from 1 up to
 *          n.
 * https://oeis.org/A214701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 245, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A214701: number of distinct values of σ(k)/k for 1≤k≤n */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Helper struct to store reduced fractions */
struct frac_t {
    seq_t num;
    seq_t den;
};

/* Cached data for repeated calls.  a_cache[n] holds a(n),
   sigma_cache[n] holds σ(n).  max_computed is the largest n
   for which the caches have been filled. */
static idx_t max_computed = 0;
static seq_t *a_cache = NULL;
static seq_t *sigma_cache = NULL;

/* Recompute a(n) and σ(n) for all 1≤k≤n when a larger n is requested.
   The algorithm runs in O(n log n) time and uses only O(n) memory. */
static void recompute(idx_t n)
{
    if (n <= max_computed) return;           /* nothing to do */

    /* Allocate or grow the cache arrays to hold indices up to n (inclusive) */
    size_t new_cap = n + 1;                  /* indices 0..n */
    a_cache     = (seq_t *)realloc(a_cache,     new_cap * sizeof(seq_t));
    sigma_cache = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));

    /* Initialise σ(k) with 1 (the divisor 1) for all k≥1 */
    for (idx_t i = 1; i <= n; ++i) sigma_cache[i] = (seq_t)1;

    /* Standard divisor‑sum sieve: add each divisor i to all multiples of i */
    for (idx_t i = 2; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            sigma_cache[j] += (seq_t)i;

    /* Array of unique reduced fractions σ(k)/k */
    struct frac_t *fracs = (struct frac_t *)malloc(sizeof(struct frac_t) * n);
    size_t fracs_used = 0;
    seq_t distinct = 0;

    /* Compute a(k) for each k=1..n */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t num = sigma_cache[k];
        seq_t den = (seq_t)k;
        seq_t g   = gcd(num, den);
        num /= g;
        den /= g;

        /* Linear search for the reduced fraction */
        bool found = false;
        for (size_t t = 0; t < fracs_used; ++t)
            if (fracs[t].num == num && fracs[t].den == den) {
                found = true;
                break;
            }

        if (!found) {
            fracs[fracs_used].num = num;
            fracs[fracs_used].den = den;
            ++fracs_used;
            ++distinct;
        }
        a_cache[k] = distinct;
    }

    free(fracs);
    max_computed = n;
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    recompute(n);
    return a_cache[n];
}
