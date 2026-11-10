/*
 * A056835  a(n) = lcm(s(1),...,s(n)) where {s(n)} = A024619 and a(0) = 1.
 * https://oeis.org/A056835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm(s(1),...,s(n)), s = A024619 (non-prime-powers:
 *            6,10,12,14,15,18,...), a(0) = 1; computed by factoring each term
 *            and tracking max prime powers
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 24, u64 n <= 58, u128 n <= 122,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 3925 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    if (n == 0) return result;

    uint64_t limit = (uint64_t)n * 2 + 100;
    uint64_t *maxpow = (uint64_t *)calloc((size_t)(limit + 1), sizeof(uint64_t));
    if (!maxpow) return 0;

    uint64_t count = 0;
    for (uint64_t m = 2; m <= limit && count < (uint64_t)n; ++m) {
        uint64_t rem = m;
        uint64_t pp[20];
        uint64_t pr[20];
        int k = 0;
        for (uint64_t p = 2; p * p <= rem; ++p) {
            if (rem % p == 0) {
                uint64_t v = 1;
                while (rem % p == 0) { rem /= p; v *= p; }
                pr[k] = p; pp[k] = v; ++k;
            }
        }
        if (rem > 1) { pr[k] = rem; pp[k] = rem; ++k; }
        if (k < 2) continue; /* prime power */
        ++count;
        for (int i = 0; i < k; ++i) {
            if (pp[i] > maxpow[pr[i]]) maxpow[pr[i]] = pp[i];
        }
    }

    for (uint64_t p = 2; p <= limit; ++p) {
        if (maxpow[p]) result *= (seq_t)maxpow[p];
    }
    free(maxpow);
    return result;
}
