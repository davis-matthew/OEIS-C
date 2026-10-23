/*
 * A024173  Integer part of ((4th elementary symmetric function of
 *          1,2,..,n)/(2nd elementary symmetric function of 1,2,...,n)).
 * https://oeis.org/A024173
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( ( (n-3)*(n-2)*(15*n^3 + 15*n^2 - 10*n - 8) ) /
 *            (240*(2 + 3*n)) )
 * Range:     u8 n <= 11, u16 n <= 43, u32 n <= 674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For lower indices return 0. */
    if (n < 2) return 0;

    /* Use signed 128‑bit integers for safe intermediate calculations. */
    __int128 nn     = (__int128)n;                 /* n as int128   */
    __int128 n3     = nn - 3;                      /* n-3           */
    __int128 n2     = nn - 2;                      /* n-2           */
    __int128 term   = 15*nn*nn*nn + 15*nn*nn - 10*nn - 8;
    __int128 num    = n3 * n2 * term;
    __int128 denom  = (__int128)240 * (2 + 3*nn);

    /* numerator and denominator are non‑negative for n ≥ 2,
       so integer division gives the required floor. */
    __int128 q = num / denom;

    return (seq_t)q;
}
