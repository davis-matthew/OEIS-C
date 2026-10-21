/*
 * A052267  Number of 2 X n matrices over GF(3) under row and column
 *          permutations.
 * https://oeis.org/A052267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((315*(475+37*(-1)^n) + 6*(54959+945*(-1)^n)*n +
 *            (298618+630*(-1)^n)*n^2 + 150528*n^3 + 46788*n^4 + 9156*n^5 +
 *            1092*n^6 + 72*n^7 + 2*n^8)) / 161280
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses 128‑bit intermediate values to avoid overflow when n is large. */
seq_t a(idx_t n)
{
    /* (-1)^n : +1 for even n, -1 for odd n */
    __int128 sgn = (n & 1) ? -1 : 1;

    /* Use 128‑bit arithmetic throughout the calculation */
    __int128 nn = ( __int128 ) n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;
    __int128 n7 = n6 * nn;
    __int128 n8 = n7 * nn;

    __int128 numerator = 0;

    numerator += (__int128)315 * (475 + 37 * sgn);
    numerator += (__int128)6   * (54959 + 945 * sgn) * nn;
    numerator += (__int128)(298618 + 630 * sgn) * n2;
    numerator += (__int128)150528 * n3;
    numerator += (__int128)46788  * n4;
    numerator += (__int128)9156   * n5;
    numerator += (__int128)1092   * n6;
    numerator += (__int128)72     * n7;
    numerator += (__int128)2      * n8;

    const __int128 denom = 161280;
    __int128 result = numerator / denom;

    return (seq_t)result;
}
