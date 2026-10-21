/*
 * A220749  Equals one maps: number of n X 4 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal and vertical neighbors in a random 0..3 n X 4 array.
 * https://oeis.org/A220749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=8, a(2)=176, a(3)=3778, a(4)=65084, a(5)=1048014 a(n) =
 *            -312 + 16^n - 36*n for n > 5 (empirical: a(n) = 18*a(n-1) -
 *            33*a(n-2) + 16*a(n-3) for n>8)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n <= 5) {
        switch ((int)n) {
        case 1: return (seq_t)8;
        case 2: return (seq_t)176;
        case 3: return (seq_t)3778;
        case 4: return (seq_t)65084;
        case 5: return (seq_t)1048014;
        }
    }

    /* For n > 5 use the closed‑form expression:
         a(n) = 16^n - 36*n - 312                           */
    seq_t result = pow_seq((seq_t)16, (int)n);   /* 16^n  */
    result -= (seq_t)36 * (seq_t)n;              /* subtract 36*n */
    result -= (seq_t)312;                       /* subtract 312  */
    return result;
}
