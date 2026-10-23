/*
 * A139326  Write the first n^2 odd numbers consecutively in n rows of length
 *          n: a(n) = minimal number of primes in a row.
 * https://oeis.org/A139326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal number of primes in any row when the * first n^2
 *            odd numbers are written in an n×n table
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

static inline void set_bit(unsigned char *b, size_t i) {
    b[i >> 3] |= (1U << (i & 7));
}
static inline void clear_bit(unsigned char *b, size_t i) {
    b[i >> 3] &= ~(1U << (i & 7));
}
static inline int test_bit(const unsigned char *b, size_t i) {
    return (b[i >> 3] >> (i & 7)) & 1;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* offset handling */
    unsigned long long limit = 2ULL * n * n;   /* highest odd needed: 2*n^2-1 */
    size_t N = (limit - 1) / 2 + 1;          /* number of odd numbers from 1 to limit */
    size_t bits_size = (N + 7) >> 3;
    unsigned char *bits = (unsigned char *)malloc(bits_size);
    if (!bits) return 0;                    /* allocation failure unlikely */
    memset(bits, 0xFF, bits_size);          /* all odds initially prime */
    if (N > 0) clear_bit(bits, 0);           /* 1 is not prime */

    unsigned long long sqrt_limit = sqrt((long double)limit);
    size_t sqrt_idx = (sqrt_limit - 1) / 2;
    for (size_t i = 1; i <= sqrt_idx; ++i) {
        if (test_bit(bits, i)) {
            unsigned long long p = 1ULL + 2ULL * i;          /* the prime itself */
            unsigned long long start = (p * p - 1) / 2;      /* index of p^2 */
            for (unsigned long long j = start; j < N; j += p)
                clear_bit(bits, (size_t)j);
        }
    }

    seq_t min_primes = n;                   /* upper bound: all primes in a row */
    for (idx_t r = 0; r < n; ++r) {
        seq_t row_primes = 0;
        idx_t base = r * n;
        for (idx_t c = 0; c < n; ++c) {
            if (test_bit(bits, base + c))
                ++row_primes;
        }
        if (row_primes < min_primes)
            min_primes = row_primes;
    }

    free(bits);
    return min_primes;
}
