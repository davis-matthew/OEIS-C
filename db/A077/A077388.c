/*
 * A077388  Row sums of the triangle in A122820.
 * https://oeis.org/A077388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the first n consecutive primes (least start) whose
 *            sum is divisible by n; found by search with a prime sieve and a
 *            sliding window
 * Range:     u8 n <= 4, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    uint64_t limit = 2000;
    if ((uint64_t)n * 30 > limit) limit = (uint64_t)n * 30;
    for (;;) {
        char *comp = (char *)calloc((size_t)limit + 1, 1);
        if (!comp) exit(EXIT_FAILURE);
        size_t cnt = 0;
        uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)limit / 2 + 2));
        if (!pr) exit(EXIT_FAILURE);
        for (uint64_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                pr[cnt++] = i;
                for (uint64_t j = i * i; j <= limit && i * i <= limit; j += i) comp[j] = 1;
            }
        }
        free(comp);
        if (cnt >= (size_t)n) {
            seq_t sum = 0;
            size_t w = (size_t)n;
            for (size_t i = 0; i < w; i++) sum += (seq_t)pr[i];
            seq_t nn = (seq_t)n;
            size_t k = 0;
            for (;;) {
                if (sum % nn == 0) {
                    free(pr);
                    return sum;
                }
                if (k + w >= cnt) break;
                sum = sum + (seq_t)pr[k + w] - (seq_t)pr[k];
                k++;
            }
        }
        free(pr);
        limit *= 2;
    }
}
