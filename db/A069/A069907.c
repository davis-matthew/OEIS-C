/*
 * A069907  Number of hexagons that can be formed with perimeter n. In other
 *          words, partitions of n into six parts such that the sum of any 5
 *          is more than the sixth.
 * https://oeis.org/A069907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(
 *            (n+15)*(39*n^4+225*n^3+1145*n^2-6375*n+95865)/4147200 + (n mod
 *            2)*(n^4+22*n^3-170*n^2-2721*n)/92160 + ((2*n^2+1) mod 3)*n/54 +
 *            ((n^2+3*n+2) mod 4)*n/256 ); The numerator is computed with a
 *            common denominator 4 147 200
 * Range:     u8 n <= 26, u16 n <= 88, u32 n <= 850, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Work with unsigned 128‑bit integers for safety. */
    uint64_t n64 = (uint64_t)n;

    /* -------- term 1: (n+15)*poly1 / 4 147 200 -------- */
    unsigned __int128 poly1 =
        (unsigned __int128)39 * n64 * n64 * n64 * n64
        + (unsigned __int128)225 * n64 * n64 * n64
        + (unsigned __int128)1145 * n64 * n64
        - (unsigned __int128)6375 * n64
        + (unsigned __int128)95865;

    unsigned __int128 term1 = ( (unsigned __int128)(n64 + 15) ) * poly1;

    /* -------- term 2: ((n mod 2)*poly2) * 45 / 4 147 200 -------- */
    unsigned __int128 poly2 =
        (unsigned __int128)n64 * n64 * n64 * n64
        + (unsigned __int128)22 * n64 * n64 * n64
        - (unsigned __int128)170 * n64 * n64
        - (unsigned __int128)2721 * n64;

    unsigned __int128 term2 = ( (n64 & 1ULL) ? 1ULL : 0ULL ) * poly2 * 45;

    /* -------- term 3: ((2*n^2+1) mod 3) * n * 76800 / 4 147 200 -------- */
    uint64_t mod3 = (2ULL * n64 * n64 + 1ULL) % 3ULL;
    unsigned __int128 term3 = (unsigned __int128)mod3 * n64 * 76800;

    /* -------- term 4: ((n^2+3*n+2) mod 4) * n * 16200 / 4 147 200 -------- */
    uint64_t mod4 = (n64 * n64 + 3ULL * n64 + 2ULL) % 4ULL;
    unsigned __int128 term4 = (unsigned __int128)mod4 * n64 * 16200;

    unsigned __int128 total = term1 + term2 + term3 + term4;

    /* The common denominator is 4 147 200 */
    seq_t result = (seq_t)(total / 4147200U);
    return result;
}
