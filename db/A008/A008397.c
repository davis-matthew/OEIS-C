/*
 * A008397  Coordination sequence for E_7 lattice.
 * https://oeis.org/A008397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/5)*(74*n^6 - 6*n^5 + 130*n^4 + 30*n^3 + 106*n^2 - 24*n
 *            + 5)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 22, u64 n <= 924, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* use 128‑bit arithmetic for intermediate values to avoid overflow */
    __int128 ns = n;
    __int128 n2 = ns * ns;
    __int128 n3 = n2 * ns;
    __int128 n4 = n3 * ns;
    __int128 n5 = n4 * ns;
    __int128 n6 = n5 * ns;

    __int128 tmp = (__int128)74 * n6
                 - (__int128)6  * n5
                 + (__int128)130 * n4
                 + (__int128)30  * n3
                 + (__int128)106 * n2
                 - (__int128)24 * ns
                 + 5;

    __int128 res = (tmp * 2) / 5;
    return (seq_t)res;
}
