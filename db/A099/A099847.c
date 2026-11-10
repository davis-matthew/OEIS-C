/*
 * A099847  Bisection of A000961.
 * https://oeis.org/A099847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000961(2*n); computed by a sieve of prime powers with a
 *            doubling limit
 * Range:     u8 n <= 35, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t A000961_a(idx_t n) {
    uint64_t limit = 4096;
    for (;;) {
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        bool *pp = (bool *)malloc((limit + 1) * sizeof(bool));
        memset(is_prime, 1, (limit + 1) * sizeof(bool));
        memset(pp, 0, (limit + 1) * sizeof(bool));
        is_prime[0] = false;
        is_prime[1] = false;
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (is_prime[i]) {
                for (uint64_t j = i * i; j <= limit; j += i) is_prime[j] = false;
            }
        }
        pp[1] = true;
        for (uint64_t p = 2; p <= limit; p++) {
            if (is_prime[p]) {
                uint64_t power = p;
                for (;;) {
                    pp[power] = true;
                    if (power > limit / p) break;
                    power *= p;
                }
            }
        }
        uint64_t count = 0;
        uint64_t result = 0;
        for (uint64_t i = 1; i <= limit; i++) {
            if (pp[i]) {
                count++;
                if (count == (uint64_t)n) {
                    result = i;
                    break;
                }
            }
        }
        free(is_prime);
        free(pp);
        if (result) return result;
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    return (seq_t)A000961_a(2 * n);
}
