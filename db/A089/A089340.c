/*
 * A089340  Index of the first occurrence of n in A089339.
 * https://oeis.org/A089340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = least m such that A089339(m) = n (search over m;
 *            A089339(m) counts primes of form concat(m,p), p prime < m)
 * Range:     u8 n <= 18, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    uint64_t *primes = NULL;
    size_t cnt = 0, cap = 0;
    uint64_t target = (uint64_t)n;
    uint64_t result = 0;

    for (uint64_t m = 1; m < 2000000ULL; ++m) {
        /* primes list holds all primes < m */
        if (m >= 3 && is_prime_u64(m - 1)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                primes = (uint64_t *)realloc(primes, cap * sizeof(uint64_t));
            }
            primes[cnt++] = m - 1;
        }
        uint64_t c = 0;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t pw = 1, t = p;
            while (t > 0) { pw *= 10; t /= 10; }
            if (is_prime_u64(m * pw + p)) {
                ++c;
                if (c > target) break;
            }
        }
        if (c == target) { result = m; break; }
    }
    free(primes);
    return (seq_t)result;
}
