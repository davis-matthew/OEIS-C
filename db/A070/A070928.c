/*
 * A070928  Smallest integer >= 0 of the form x^4 - n^3.
 * https://oeis.org/A070928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceiling(n^(3/4))^4 - n^3
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A070928 using only integer arithmetic.
   No external dependencies are required. */

#include <stdint.h>

/* integer square function used only for exponentiations */
static seq_t pow4(seq_t v)
{
    seq_t v2 = v * v;
    return v2 * v2;
}

/* floor fourth‑root of a non‑negative integer num:  largest x with x^4 ≤ num */
static seq_t floor_fourth_root(seq_t num)
{
    seq_t low  = 0;
    seq_t high = num;
    /* binary search on [0,high] */
    while ((low + 1) < high)
    {
        seq_t mid = low + ((high - low) >> 1);
        seq_t mid4 = pow4(mid);
        if (mid4 <= num)
            low = mid;
        else
            high = mid;
    }
    /* the tighter bound (high) may still satisfy mid^4 ≤ num */
    return (pow4(high) <= num) ? high : low;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* Convert the index once to seq_t */
    seq_t nn = (seq_t)n;

    /* n^3 = n * n * n */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* floor fourth root of n^3 */
    seq_t f = floor_fourth_root(n3);

    /* check if n^3 is already a perfect fourth power */
    seq_t f4 = pow4(f);

    if (f4 == n3)
        return (seq_t)0;          /* n is a power of 4 (or n^3 is a perfect fourth power) */

    /* otherwise, compute (f+1)^4 - n^3 */
    seq_t fp = f + (seq_t)1;
    seq_t fp4 = pow4(fp);
    return fp4 - n3;
}
