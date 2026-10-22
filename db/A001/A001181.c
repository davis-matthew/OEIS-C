/*
 * A001181  Number of Baxter permutations of length n (also called Baxter
 *          numbers).
 * https://oeis.org/A001181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   (n+2)*(n+3)*a(n) = (7*n^2+7*n-2)*a(n-1) + 8*(n-1)*(n-2)*a(n-2),
 *            a(0) = a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t p2 = 1, p1 = 1;
    idx_t i;
    if (n < 2) return 1;
    for (i = 2; i <= n; i++) {
        uint64_t c1 = 7 * (uint64_t)i * i + 7 * (uint64_t)i - 2;
        uint64_t c2 = 8 * ((uint64_t)i - 1) * ((uint64_t)i - 2);
        uint64_t d = ((uint64_t)i + 2) * ((uint64_t)i + 3);
        seq_t sd = (seq_t)d;
        /* split previous terms as q*d + r so no intermediate exceeds the result */
        seq_t q1 = p1 / sd, q2 = p2 / sd;
        uint64_t r1 = (uint64_t)(p1 % sd);
        uint64_t r2 = (uint64_t)(p2 % sd);
        unsigned __int128 s = (unsigned __int128)c1 * r1 + (unsigned __int128)c2 * r2;
        uint64_t sq = (uint64_t)(s / d);
        seq_t cur = (seq_t)c1 * q1 + (seq_t)c2 * q2 + (seq_t)sq;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
