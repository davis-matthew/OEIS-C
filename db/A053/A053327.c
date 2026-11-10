/*
 * A053327  First differences of A031936.
 * https://oeis.org/A053327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A031936(n+1) - A031936(n); computed by sieving primes and
 *            finding consecutive prime pairs with gap 18
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9056 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    uint64_t limit = 1ULL << 20;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc((size_t)(limit + 1), 1);
        if (!comp)
            return (seq_t)0;
        for (uint64_t i = 2; i * i <= limit; i++) {
            if (!comp[i]) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    comp[j] = 1;
            }
        }
        idx_t count = 0;
        uint64_t prev_lower = 0;
        uint64_t last = 0;
        for (uint64_t i = 2; i <= limit; i++) {
            if (comp[i])
                continue;
            if (last != 0 && i - last == 18ULL) {
                count++;
                if (count == n + 1) {
                    free(comp);
                    return (seq_t)(last - prev_lower);
                }
                prev_lower = last;
            }
            last = i;
        }
        free(comp);
        limit <<= 1;
    }
}
