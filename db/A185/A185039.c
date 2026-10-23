/*
 * A185039  Numbers of the form 9*m^2 + 4*m, m an integer.
 * https://oeis.org/A185039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (18*n*(n-1)+(2*n-1)*(-1)^n+1)/8
 * Range:     u8 n <= 11, u16 n <= 171, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 128‑bit signed intermediate keeps multiplications and
     * subtractions safe even for large indices. */
    __int128 nn   = n;
    __int128 prod = 18 * nn * (nn - 1);   /* 18*n*(n‑1) */

    if ((n & 1) == 0)        /* even n */
        prod += 2 * nn;      /* +2*n */
    else                     /* odd n */
    {
        prod -= 2 * nn;      /* -2*n */
        prod += 2;           /* +2 */
    }

    /* division by 8 (right shift) */
    __int128 res = prod >> 3;
    return (seq_t)res;
}
