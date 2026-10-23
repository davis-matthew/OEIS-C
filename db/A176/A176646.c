/*
 * A176646  a(n) is the number of convex pentagons in an n-triangular net.
 * https://oeis.org/A176646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/320)*(12*n^5 - 10*n^4 - 60*n^3 + 40*n^2 + 48*n - 15 +
 *            15*(-1)^n)
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 163, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* safe guard for offset 1 */

    if ((n & 1) == 0)                    /* n is even: a(2*m) */
    {
        idx_t m = n >> 1;                /* m = n/2 */
        __int128 prod = (__int128)m * (4*m - 3);
        prod *= (3*m + 1);
        prod *= (m - 1);
        prod *= (m + 1);
        prod /= 10;                      /* division by 10 as in the formula */
        return (seq_t)prod;
    }
    else                                 /* n is odd: a(2*m+1) */
    {
        idx_t m = (n - 1) >> 1;          /* m = (n-1)/2 */
        __int128 part = (__int128)12 * m * m * m;
        part += (__int128)13 * m * m;
        part -= (__int128)8 * m;
        part -= 2;
        __int128 prod = (__int128)m * (m + 1) * part;
        prod /= 10;
        return (seq_t)prod;
    }
}
