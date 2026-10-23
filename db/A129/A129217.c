/*
 * A129217  Permutations with exactly 9 fixed points.
 * https://oeis.org/A129217
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,9) * derangement(n - 9)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 17, u64 n <= 25, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where derangement(m) = number of derangements of m distinct items */

#include <stddef.h>            /* for size_t */
#include "../utils/binom.h"    /* for binom() */

/* Compute the number of derangements D(m) for m ≥ 0. */
static seq_t derangement(idx_t m)
{
    if (m == 0) return (seq_t)1;   /* D(0) = 1 */
    if (m == 1) return (seq_t)0;   /* D(1) = 0 */

    seq_t d0 = (seq_t)1;           /* D(0) */
    seq_t d1 = (seq_t)0;           /* D(1) */

    for (idx_t i = 2; i <= m; ++i) {
        seq_t d = (seq_t)(i - 1) * (d1 + d0);
        d0 = d1;
        d1 = d;
    }
    return d1;
}

seq_t a(idx_t n)
{
    if (n < 9)                     /* offset of the sequence is 9 */
        return (seq_t)0;

    seq_t binom9 = binom(n, (idx_t)9);          /* choose(n,9) */
    seq_t der     = derangement(n - (idx_t)9);   /* D(n-9)         */

    return binom9 * der;
}
