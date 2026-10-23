/*
 * A129238  Permutations with exactly 11 fixed points.
 * https://oeis.org/A129238
 *
 * Offset:    11    (a(11) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,11) * derangement(n-11) where derangement(k) is
 *            the number of derangements of a k‑element set (subsequence of
 *            A000166)
 * Range:     u8 n <= 13, u16 n <= 15, u32 n <= 19, u64 n <= 26, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

/* Return the k-th derangement number.  Works for any seq_t that
   supports +, -, *, and integer casting from (idx_t). */
static seq_t derangement(idx_t k)
{
    if (k == 0) return (seq_t)1;          /* D(0) = 1 */
    if (k == 1) return (seq_t)0;          /* D(1) = 0 */

    seq_t d0 = (seq_t)1;                  /* D(0) */
    seq_t d1 = (seq_t)0;                  /* D(1) */
    for (idx_t i = 2; i <= k; ++i)
    {
        /* D(i) = (i-1)*(D(i-1)+D(i-2)) */
        seq_t sum = d1 + d0;
        seq_t di  = (seq_t)(i - 1) * sum;
        d0 = d1;
        d1 = di;
    }
    return d1;                            /* D(k) */
}

seq_t a(idx_t n)
{
    /* OEIS offset: valid for n >= 11 only.  For n < 11 the sequence is
       defined to be 0. */
    if (n < 11) return (seq_t)0;

    idx_t k = n - 11;                     /* size of non‑fixed part   */
    seq_t der = derangement(k);           /* derangement of k elements */
    seq_t bin = binom(n, 11);             /* choose the 11 fixed points */
    return bin * der;                     /* total permutations          */
}
