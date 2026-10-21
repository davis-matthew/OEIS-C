/*
 * A081494  Start with Pascal's triangle; form a triangle by sliding down n
 *          steps from top on both sides and including the horizontal row,
 *          deleting the inner numbers; a(n) = sum of entries on perimeter of
 *          triangle.
 * https://oeis.org/A081494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 1; for n > 1, a(n) = 2^(n-1) + 2*n - 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute 2^e by repeated left shifts; safe for small e and keeps the
   result within the bounds of seq_t.  For larger exponents the value
   will not fit in seq_t anyway, but we still perform the shifts
   (UB only occurs when shifting by the full width, which we never do). */
static seq_t pow2_idx(idx_t e)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < e; ++i)
        r <<= 1;               /* shift by 1 each iteration */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)            /* offset 1; first term is 1 */
        return (seq_t)1;

    /* a(n) = 2^(n-1) + 2*n - 3 for n > 1 */
    seq_t power = pow2_idx(n - 1);        /* 2^(n-1) */
    seq_t add   = (seq_t)((seq_t)2 * (seq_t)n - (seq_t)3);
    return power + add;
}
