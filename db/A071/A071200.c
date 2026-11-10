/*
 * A071200  Primes which do not appear in A071196.
 * https://oeis.org/A071200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p that is not equal to A071196(m) for any m,
 *            where A071196(m) is the first prime among the sums of the
 *            consecutive primes prime(m)+...+prime(m+k), k>=1. Computed by a
 *            sieve and a search
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    uint64_t L = 1024;
    for (;;) {
        uint8_t *comp = (uint8_t *)calloc((size_t)L + 1, 1);
        uint8_t *mark = (uint8_t *)calloc((size_t)L + 1, 1);
        uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * ((size_t)L / 2 + 2));
        size_t m = 0;
        if (!comp || !mark || !pr) abort();
        for (uint64_t i = 2; i <= L; i++) {
            if (!comp[i]) {
                pr[m++] = i;
                for (uint64_t j = i * i; j <= L; j += i) comp[j] = 1;
            }
        }
        for (size_t i = 0; i < m && pr[i] * 2 <= L; i++) {
            uint64_t sum = pr[i];
            for (size_t j = i + 1; j < m; j++) {
                sum += pr[j];
                if (sum > L) break;
                if (is_prime_u64(sum)) { mark[sum] = 1; break; }
            }
        }
        idx_t cnt = 0;
        uint64_t result = 0;
        bool found = false;
        for (size_t i = 0; i < m; i++) {
            if (!mark[pr[i]]) {
                cnt++;
                if (cnt == n) { result = pr[i]; found = true; break; }
            }
        }
        free(comp);
        free(mark);
        free(pr);
        if (found) return (seq_t)result;
        L *= 4;
    }
}
