/*
 * A110694  Kekulé numbers for certain benzenoids.
 * https://oeis.org/A110694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*(34*n^3 + 199*n^2 + 355*n +
 *            210)/7!
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 u = n;                           /* cast index to 128‑bit interim type */
    unsigned __int128 poly = (unsigned __int128)34 * u * u * u
                          + (unsigned __int128)199 * u * u
                          + (unsigned __int128)355 * u
                          + 210;                          /* constant 210 */

    unsigned __int128 prod = (u + 1) * (u + 2) * (u + 3) * (u + 4) * poly;
    prod /= 5040u;                                     /* 7! = 5040 */

    return (seq_t)prod;
}
