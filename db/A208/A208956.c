/*
 * A208956  Triangular array read by rows. T(n,k) is the number of
 *          n-permutations that have at least k fixed points with n >= 1 and 1
 *          <= k <= n.
 * https://oeis.org/A208956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = flatten(T(r,k)) where r(r‑1)/2 < n ≤ r(r+1)/2, k = n –
 *            r(r‑1)/2, and T(r,k) = Σ_{i=k..r} binom(r,i) * D(r‑i) (D(m) =
 *            number of derangements of m)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 91, u64 n <= 210,
 *            u128 n <= 595, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"

/* Derangement cache.  der_cache[0] = 1, der_cache[1] = 0. */
static seq_t *der_cache = NULL;
static idx_t der_current = 0;

/* Ensure derangements up to `upto` are in der_cache. */
static void ensure_der(idx_t upto)
{
    if (upto <= der_current) return;            /* already computed */

    if (der_cache == NULL) {                    /* first allocation */
        der_cache = (seq_t *)malloc(sizeof(seq_t) * (upto + 1));
        if (der_cache == NULL) return; /* allocation failure – should not happen */
        der_cache[0] = (seq_t)1;
        if (upto >= 1) der_cache[1] = (seq_t)0;
        der_current = upto;
        return;
    }

    /* enlarge cache */
    seq_t *tmp = (seq_t *)realloc(der_cache,
                                  sizeof(seq_t) * (upto + 1));
    if (tmp == NULL) return; /* allocation failure – ignore */
    der_cache = tmp;

    /* compute new entries */
    for (idx_t i = der_current + 1; i <= upto; ++i) {
        seq_t sum = der_cache[i - 1] + der_cache[i - 2];
        der_cache[i] = (seq_t)(i - 1) * sum;
    }
    der_current = upto;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0, harmless */

    /* Locate the row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Compute the column (1‑based). */
    idx_t prev_total = (r - 1) * r / 2;
    idx_t k = n - prev_total; /* 1 ≤ k ≤ r */

    /* Make sure we have the necessary derangements. */
    ensure_der(r);

    /* Sum i = k..r of C(r,i)*D(r-i). */
    seq_t result = (seq_t)0;
    for (idx_t i = k; i <= r; ++i) {
        seq_t bin = binom(r, i);
        seq_t der = der_cache[r - i];
        result += bin * der;
    }
    return result;
}
