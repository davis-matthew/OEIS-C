/*
 * A055201  Numbers k such that A005728(k) is prime.
 * https://oeis.org/A055201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k such that A005728(k) = 1 + Sum_{i=1..k} phi(i) is
 *            prime; computed by a totient sieve and a primality test on each
 *            partial sum
 * Range:     u8 n <= 75, u16 n <= 7000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9644 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    idx_t limit = 32 * n + 64;
    for (;;) {
        uint64_t *phi = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
        if (!phi) return 0;
        for (idx_t i = 0; i <= limit; i++) phi[i] = i;
        for (idx_t i = 2; i <= limit; i++) {
            if (phi[i] == i) {
                for (idx_t j = i; j <= limit; j += i) phi[j] -= phi[j] / i;
            }
        }
        uint64_t sum = 1;
        idx_t count = 0;
        for (idx_t k = 1; k <= limit; k++) {
            sum += phi[k];
            if (is_prime_u64(sum)) {
                count++;
                if (count == n) {
                    free(phi);
                    return (seq_t)k;
                }
            }
        }
        free(phi);
        limit *= 2;
    }
}
