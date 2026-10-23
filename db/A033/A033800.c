/*
 * A033800  Product t2(q^d); d | 41, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033800
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 300, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033800: number of positive odd solutions (x,y) to
   x^2 + 41*y^2 = 8*n + 42.  The implementation is based on the
   equivalence a(a+1) + 41*b(b+1) = 2*n where
   x = 2*a+1, y = 2*b+1.  For each n the function counts all
   non‑negative integer pairs (a,b) satisfying this equation. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = ((seq_t)n) * 2;          /* 2*n  */
    seq_t count = 0;

    for (idx_t b = 0; ; ++b)
    {
        seq_t b_b1 = (seq_t)b * (b + 1);    /* b*(b+1)   */
        seq_t b_val = b_b1 * 41;            /* 41*b*(b+1) */
        if (b_val > tn)
            break;                          /* no more b possible */

        seq_t val = tn - b_val;             /* a(a+1) part */
        seq_t D   = 1 + (val * 4);          /* discriminant */

        seq_t sqrt_D = floor_sqrt(D);
        if (sqrt_D * sqrt_D == D && (sqrt_D & 1))
        {
            /* sqrt_D is odd, then a = (sqrt_D - 1) / 2 is an integer */
            ++count;
        }
    }

    return count;
}
