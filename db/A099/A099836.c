/*
 * A099836  Bisection of A000961.
 * https://oeis.org/A099836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000961(2n+1); computed by a sieve of primes, marking
 *            prime powers, with the limit doubled until enough are found
 * Range:     u8 n <= 34, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 56 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t target = 2 * n + 1;
    idx_t limit = 4096;
    for (;;) {
        unsigned char *comp = (unsigned char *)malloc((size_t)(limit + 1));
        unsigned char *pp = (unsigned char *)malloc((size_t)(limit + 1));
        memset(comp, 0, (size_t)(limit + 1));
        memset(pp, 0, (size_t)(limit + 1));
        pp[1] = 1;
        for (idx_t i = 2; i <= limit; i++) {
            if (!comp[i]) {
                if (i <= limit / i) {
                    for (idx_t j = i * i; j <= limit; j += i) comp[j] = 1;
                }
                idx_t power = i;
                for (;;) {
                    pp[power] = 1;
                    if (power > limit / i) break;
                    power *= i;
                }
            }
        }
        idx_t count = 0;
        for (idx_t i = 1; i <= limit; i++) {
            if (pp[i]) {
                count++;
                if (count == target) {
                    free(comp);
                    free(pp);
                    return (seq_t)i;
                }
            }
        }
        free(comp);
        free(pp);
        limit *= 2;
    }
}
