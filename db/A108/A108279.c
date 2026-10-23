/*
 * A108279  a(n) = number of squares with corners on an n X n grid, distinct
 *          up to congruence.
 * https://oeis.org/A108279
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values of a^2+b^2 for integers a,b ≥ 0
 *            with a+b ≤ n-1, not both zero
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    unsigned int N = (unsigned int)(n - 1);
    unsigned int maxS = N * N;                     // maximum a^2 + b^2
    unsigned int words = (maxS >> 5) + 1;           // 32 bits per word

    uint32_t *bits = (uint32_t *)malloc(words * sizeof(uint32_t));
    if (!bits) return (seq_t)0;                     // allocation failed
    for (unsigned int i = 0; i < words; ++i) bits[i] = 0;

    for (unsigned int a = 0; a <= N; ++a) {
        unsigned int max_b = N - a;
        for (unsigned int b = 0; b <= max_b; ++b) {
            if (a == 0 && b == 0) continue;        // skip zero vector
            unsigned int s = a * a + b * b;         // a^2 + b^2
            unsigned int word = s >> 5;
            bits[word] |= 1u << (s & 31);
        }
    }

    uint64_t count = 0;
    for (unsigned int i = 0; i < words; ++i)
        count += __builtin_popcount(bits[i]);

    free(bits);
    return (seq_t)count;
}
