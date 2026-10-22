/*
 * A000499  a(n) = Sum_{k=1..n-1} k^3*sigma(k)*sigma(n-k).
 * https://oeis.org/A000499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} k^3*sigma(k)*sigma(n-k)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 59, u64 n <= 2399,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t result = 0;
    uint64_t *sg;
    idx_t i, j, k;

    if (n < 2) return 0;

    sg = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    if (!sg) return 0;
    for (i = 0; i < n; i++) sg[i] = 0;
    for (i = 1; i < n; i++)
        for (j = i; j < n; j += i)
            sg[j] += i;

    for (k = 1; k < n; k++) {
        seq_t ks = (seq_t)k;
        seq_t term = ks * ks * ks;
        term *= (seq_t)sg[k];
        term *= (seq_t)sg[n - k];
        result += term;
    }

    free(sg);
    return result;
}
