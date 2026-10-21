/*
 * A059246  Numerator of Sum_{j=1..n} d(j)/n, where d = number of divisors
 *          function (A000005).
 * https://oeis.org/A059246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{j=1..n} d(j)/n, i.e. a(n) = S(n) /
 *            gcd(S(n), n) where S(n) = Σ_{j=1}^n d(j)
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *psum = NULL;        /* psum[i] = Σ_{j=1..i} d(j) */
static uint32_t *divcnt = NULL;   /* divcnt[i] = d(i) */
static idx_t max_prepped = 0;

/* Make sure psum and divcnt are ready for all indices up to n. */
static void ensure_prepped(idx_t n)
{
    if (n <= max_prepped) return;

    idx_t len = n + 1;                 /* include index 0 */
    divcnt = (uint32_t *)realloc(divcnt, len * sizeof(uint32_t));
    psum  = (seq_t *)realloc(psum,  len * sizeof(seq_t));
    if (!divcnt || !psum) abort();

    /* initialize divisor counts to zero */
    for (idx_t i = 1; i < len; ++i) divcnt[i] = 0;

    /* compute divisor counts via a simple sieve */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            ++divcnt[j];
        }
    }

    /* compute prefix sums of divisor counts */
    psum[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i) {
        psum[i] = psum[i-1] + (seq_t)divcnt[i];
    }

    max_prepped = n;
}

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 for safety */
    ensure_prepped(n);

    seq_t total = psum[n];
    seq_t g = gcd(total, (seq_t)n);
    return total / g;
}
