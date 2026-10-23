/*
 * A022154  Coordination sequence for root lattice B_12.
 * https://oeis.org/A022154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( 65344*n^11 - 179168*n^10 + 2231680*n^9 - 4643760*n^8 +
 *            22442112*n^7 - 33057024*n^6 + 76060160*n^5 - 69575440*n^4 +
 *            72405344*n^3 - 29758608*n^2 + 8915760*n ) / 155925
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 61, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* use 128‑bit integer for intermediate calculations */
    __int128 nn = (__int128)n;
    __int128 t  = 65344;
    t = t * nn - 179168;
    t = t * nn + 2231680;
    t = t * nn - 4643760;
    t = t * nn + 22442112;
    t = t * nn - 33057024;
    t = t * nn + 76060160;
    t = t * nn - 69575440;
    t = t * nn + 72405344;
    t = t * nn - 29758608;
    t = t * nn + 8915760;
    t = t * nn;               /* raise the degree to n^11 */
    return (seq_t)(t / 155925);
}
