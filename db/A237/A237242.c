/*
 * A237242  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          upper median minus the lower median minus the minimum of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 352*a(n-4) for n>5; * initial values: a(1)=81, a(2)=344,
 *            a(3)=1488, a(4)=6496, a(5)=28176. * This implementation uses the
 *            recurrence directly
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 58,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Base cases for the first five terms. */
    static const seq_t init[5] =
    {
        (seq_t)81,     /* a(1) */
        (seq_t)344,    /* a(2) */
        (seq_t)1488,   /* a(3) */
        (seq_t)6496,   /* a(4) */
        (seq_t)28176   /* a(5) */
    };

    if (n <= 5)
        return init[n - 1];

    /* Number of times we need to multiply by 352. */
    idx_t t = (n - 2) / 4;
    /* The index of the base term (must lie in [2,5]). */
    idx_t baseIdx = n - 4 * t;          /* 2 <= baseIdx <= 5 */
    seq_t base = init[baseIdx - 1];        /* 0‑based array index */

    /* Compute 352^t (using the helper function). */
    seq_t pow352 = seq_pow((seq_t)352, t);

    /* Final value: base * 352^t. */
    return pow352 * base;
}
