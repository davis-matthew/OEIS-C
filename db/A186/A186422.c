/*
 * A186422  First differences of A186421.
 * https://oeis.org/A186422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -((2*n+1)*(-1)^n - 2*i^(n*(n+1)) - 3)/4 where i^k cycles
 *            1, i, -1, -i; since n*(n+1) is always even, i^(n*(n+1)) equals 1
 *            if n%4==0 or n%4==3, and -1 otherwise
 * Range:     i8 n <= 62, i16 n <= 83, i32 n <= 83, i64 n <= 83, i128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (-1)^n : 1 for even n, -1 for odd n */
    seq_t sign = ((n & 1) == 0) ? 1 : -1;

    /* i^(n*(n+1)) : 1 for n%4==0 or 3, -1 for n%4==1 or 2 */
    seq_t powi = ((n & 3) == 0 || (n & 3) == 3) ? 1 : -1;

    /* 2*n+1 as seq_t */
    seq_t two_n_plus_one = (seq_t)n * 2 + 1;

    /* numerator = (2*n+1)*sign - 2*powi - 3 */
    seq_t numerator = two_n_plus_one * sign - (seq_t)2 * powi - 3;

    /* return -numerator / 4 (guaranteed integer division) */
    return -numerator / 4;
}
