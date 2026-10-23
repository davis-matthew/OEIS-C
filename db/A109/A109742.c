/*
 * A109742  a(n) = d(n-1) + d(n-2) + (n-1)[d(n-2) + 2d(n-3) + d(n-4)], where
 *          d(n), the derangement numbers, are given in A000166. (Let d(n) = 0
 *          if n < 0.)
 * https://oeis.org/A109742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n-1) + d(n-2) + (n-1) * ( d(n-2) + 2*d(n-3) + d(n-4) ),
 *            where d(k) is the derangement number (OEIS A000166), d(k) = 0
 *            for k < 0. Initial values: a(1) = 1, a(2) = 2, etc
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Pre‑allocate a sufficient amount of derangements (up to MAXDER).
   For n ≤ 10000 this is more than enough; it also works if the
   caller requests larger n.                                                       */
#define MAXDER 100000

/* static array of derangement numbers */
static seq_t der_cache[MAXDER + 1];
static idx_t der_cache_size = 0;   /* first unused index */

/* Ensure that der_cache[0 .. k] are computed. */
static void ensure_derangement(idx_t k)
{
    /* Guard against overflow of the array bounds. */
    if (k > MAXDER) {
        /* If k exceeds the fixed bound, treat the additional
           values as zero (they are not needed for the required
           n ≤ 10000). */
        if (k < 2)
            return;
        k = MAXDER;
    }

    /* First initialise the cache for 0 and 1, if not already done. */
    if (der_cache_size == 0) {
        der_cache[0] = (seq_t)1;
        der_cache[1] = (seq_t)0;
        der_cache_size = 2;
    }

    /* Compute missing values up to k. */
    for (idx_t i = der_cache_size; i <= k; ++i) {
        /* Recurrence: d(i) = (i-1)*(d(i-1)+d(i-2)) for i ≥ 2 */
        seq_t sum = der_cache[i - 1] + der_cache[i - 2];
        der_cache[i] = (seq_t)(i - 1) * sum;
    }
    if (k + 1 > der_cache_size)
        der_cache_size = k + 1;
}

/* Retrieve d(k).  For k < 0 we return 0 (the caller never
   requests a negative index). */
static seq_t derangement(idx_t k)
{
    if (k < 0)
        return (seq_t)0;
    if ((idx_t)k >= der_cache_size)
        ensure_derangement(k);
    return der_cache[k];
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by the specification.  For safety, guard against
       overflow when computing n-1 if n == 0, but that case is
       not used. */
    seq_t d_nm1 = derangement(n - 1);
    seq_t d_nm2 = (n >= 2) ? derangement(n - 2) : (seq_t)0;
    seq_t d_nm3 = (n >= 3) ? derangement(n - 3) : (seq_t)0;
    seq_t d_nm4 = (n >= 4) ? derangement(n - 4) : (seq_t)0;

    seq_t part1 = d_nm1 + d_nm2;
    seq_t inside = d_nm2 + (seq_t)2 * d_nm3 + d_nm4;
    return part1 + (seq_t)(n - 1) * inside;
}
