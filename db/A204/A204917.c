/*
 * A204917  Least j such that n divides s(A204916(n))-s(j), where
 *          s(j)=(prime(j))^2.
 * https://oeis.org/A204917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = least j < k with prime(k)^2 == prime(j)^2 (mod n), where
 *            k = A204916(n) is the first index at which the squared primes
 *            mod n repeat (found by sieve + scan)
 * Range:     u8 n <= 1656, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9868 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t N = (size_t)n;
    size_t limit = 20 * (N + 2) + 100;

    unsigned char *comp = (unsigned char *)calloc(limit + 1, 1);
    uint32_t *first = (uint32_t *)calloc(N, sizeof(uint32_t));
    if (!comp || !first) {
        free(comp);
        free(first);
        return (seq_t)0;
    }

    seq_t result = (seq_t)0;
    size_t k = 0;
    for (size_t i = 2; i <= limit; ++i) {
        if (comp[i]) continue;
        if (i * i <= limit) {
            for (size_t m = i * i; m <= limit; m += i) comp[m] = 1;
        }
        ++k;
        uint64_t p = (uint64_t)i;
        size_t r = (size_t)((p * p) % (uint64_t)n);
        if (first[r]) {
            result = (seq_t)first[r];
            break;
        }
        first[r] = (uint32_t)k;
    }

    free(comp);
    free(first);
    return result;
}
