/*
 * A005378  The female of a pair of recurrences.
 * https://oeis.org/A005378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   F(n) = n - M(F(n-1)), M(n) = n - F(M(n-1)), F(0) = 1, M(0) = 0;
 *            a(n) = F(n)
 * Range:     u8 n <= 413, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    static uint64_t *F = NULL;
    static uint64_t *M = NULL;
    static idx_t computed = 0;
    static idx_t len = 0;

    if (F == NULL) {
        len = 128;
        while (len <= n) len *= 2;
        F = (uint64_t *)malloc(sizeof(uint64_t) * len);
        M = (uint64_t *)malloc(sizeof(uint64_t) * len);
        F[0] = 1;
        M[0] = 0;
        computed = 0;
    }

    if (n >= len) {
        idx_t newlen = len;
        while (newlen <= n) newlen *= 2;
        F = (uint64_t *)realloc(F, sizeof(uint64_t) * newlen);
        M = (uint64_t *)realloc(M, sizeof(uint64_t) * newlen);
        len = newlen;
    }

    while (computed < n) {
        idx_t i = computed + 1;
        M[i] = (uint64_t)i - F[M[i - 1]];
        F[i] = (uint64_t)i - M[F[i - 1]];
        computed = i;
    }

    return (seq_t)F[n];
}
