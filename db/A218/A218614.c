/*
 * A218614  a(n) = binary code (shown here in decimal) of the position of
 *          natural number n in the beanstalk-tree A218778.
 * https://oeis.org/A218614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; a(n) = A004754(a(n-popcount(n))) for even n,
 *            A004755(a(n-popcount(n))) for odd n (A011371(n) = n -
 *            popcount(n))
 * Range:     u8 n <= 17, u16 n <= 43, u32 n <= 103, u64 n <= 231,
 *            u128 n <= 531, big any n
 * Validated: standard, pass, 64 terms through n = 994 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;
    size_t len = 0;
    idx_t m = n;
    while (m > 1) {
        len++;
        m -= (idx_t)__builtin_popcountll((unsigned long long)m);
    }
    unsigned char *bits = (unsigned char *)malloc(len + 1);
    size_t i = 0;
    m = n;
    while (m > 1) {
        bits[i++] = (unsigned char)(m & 1);
        m -= (idx_t)__builtin_popcountll((unsigned long long)m);
    }
    seq_t x = 1;
    while (i > 0) {
        i--;
        seq_t p = 1;
        while (p != 0 && p <= x) p <<= 1;
        if (bits[i]) x = x + p;
        else x = x + p / 2;
    }
    free(bits);
    return x;
}
