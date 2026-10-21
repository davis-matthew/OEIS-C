/*
 * A208531  Number of distinct sums of subsets of the first n squares
 *          {1,4,9,...,n^2}.
 * https://oeis.org/A208531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)*(2n+1)/6 - 61 for n >= 9; a(1)=2, a(2)=4, a(3)=8,
 *            a(4)=16, a(5)=28, a(6)=52, a(7)=89, a(8)=147
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first eight terms explicitly. */
    if (n <= 8) {
        static const seq_t tbl[9] = { 0,
            2, 4, 8, 16, 28, 52, 89, 147 };
        return tbl[(size_t)n];
    }

    /* Compute the sum of the first n squares: n(n+1)(2n+1)/6. */
    seq_t nn = (seq_t)n;
    seq_t a = nn;                 /* n          */
    seq_t b = nn + 1;             /* n + 1      */
    seq_t c = (seq_t)2 * nn + 1;   /* 2n + 1     */

    /* Cancel a factor of 2.  Only one of a, b, c is even. */
    if ((a & 1) == 0)
        a >>= 1;
    else if ((b & 1) == 0)
        b >>= 1;
    else
        c >>= 1;

    /* Cancel a factor of 3.  Exactly one of a, b, c is divisible by 3. */
    if (a % 3 == 0)
        a /= 3;
    else if (b % 3 == 0)
        b /= 3;
    else
        c /= 3;

    seq_t sumsq = a * b * c;      /* now sumsq = n(n+1)(2n+1)/6 */

    return sumsq - (seq_t)61;     /* a(n) = sum of squares minus 61 */
}
