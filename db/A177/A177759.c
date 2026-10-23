/*
 * A177759  Number of ways to place 6 nonattacking bishops on an n X n
 *          toroidal board.
 * https://oeis.org/A177759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/1440 * (n-4)^2 * (n-2)^2 * n^2 * * (2*n^6 - 36*n^5 +
 *            269*n^4 - 1128*n^3 + * 3143*n^2 - 6330*n + 7425 + * (15*n^4 -
 *            240*n^3 + 1545*n^2 - 4950*n + 6975)*(-1)^n )
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 72, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n < 6 the value is 0. */

seq_t a(idx_t n)
{
    /* values for n < 6 are zero */
    if (n < 6)
        return (seq_t)0;

    /* use signed __int128 for all intermediate arithmetic to avoid overflow */
    __int128 nt    = n;
    __int128 n2    = nt * nt;
    __int128 n3    = n2 * nt;
    __int128 n4    = n3 * nt;
    __int128 n5    = n4 * nt;
    __int128 n6    = n5 * nt;

    /* factor = (n-4)^2 * (n-2)^2 * n^2 */
    __int128 factor = (nt - 4) * (nt - 4);
    factor *= (nt - 2) * (nt - 2);
    factor *= n2;

    /* polynomial part */
    __int128 p1 = 2 * n6 - 36 * n5 + 269 * n4
                  - 1128 * n3 + 3143 * n2 - 6330 * nt + 7425;

    __int128 p2 = 15 * n4 - 240 * n3 + 1545 * n2
                  - 4950 * nt + 6975;

    /* add or subtract p2 depending on parity */
    __int128 q;
    if ((n & 1) == 0)   /* n even: (-1)^n = +1 */
        q = p1 + p2;
    else                /* n odd: (-1)^n = -1 */
        q = p1 - p2;

    /* final division by 1440 */
    __int128 result = factor * q / 1440;

    return (seq_t)result;
}
