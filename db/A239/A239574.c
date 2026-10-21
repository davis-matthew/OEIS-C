/*
 * A239574  Number of non-equivalent (mod D_3) ways to place 4
 *          indistinguishable points on a triangular grid of side n so that no
 *          two of them are adjacent.
 * https://oeis.org/A239574
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^8 +4*n^7 -78*n^6 -104*n^5 +2556*n^4 -3152*n^3 *
 *            -27280*n^2 +89664*n -78336)/2304 * + IF(n == 1 mod 2)*(28*n^3
 *            -54*n^2 -160*n +129)/768 * + IF(n == 1 mod 3)*(n^2 +n -14)/18
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 41, u64 n <= 673,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the sequence using 128‑bit intermediates to avoid overflow. */
    __int128 N = n;

    /* powers of n */
    __int128 n2 = N * N;
    __int128 n3 = n2 * N;
    __int128 n4 = n2 * n2;
    __int128 n5 = n4 * N;
    __int128 n6 = n3 * n3;
    __int128 n7 = n6 * N;
    __int128 n8 = n4 * n4;

    /* polynomial part with denominator 2304 */
    __int128 poly = n8
        + (__int128)4 * n7
        - (__int128)78 * n6
        - (__int128)104 * n5
        + (__int128)2556 * n4
        - (__int128)3152 * n3
        - (__int128)27280 * n2
        + (__int128)89664 * N
        - (__int128)78336;

    /* odd‑n adjustment (denominator 768 → multiply by 3) */
    __int128 odd_num = 0;
    if (N & 1) {
        odd_num = (__int128)28 * n3
            - (__int128)54 * n2
            - (__int128)160 * N
            + (__int128)129;
    }

    /* n ≡ 1 (mod 3) adjustment (denominator 18 → multiply by 128) */
    __int128 mod3_num = 0;
    if (n % 3 == 1) {
        mod3_num = n2 + N - (__int128)14;
    }

    /* combine all terms with common denominator 2304 */
    __int128 numerator = poly
        + (__int128)3 * odd_num
        + (__int128)128 * mod3_num;

    seq_t result = (seq_t)(numerator / 2304);
    return result;
}
