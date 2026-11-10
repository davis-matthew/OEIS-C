/*
 * A072698  Sum of prime factors of A072697(n).
 * https://oeis.org/A072698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001414(A072697(n)); search over squarefree k>=2 with
 *            (sum of primes of k) divisible by omega(k), caching the sums
 * Range:     u8 n <= 111, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9980 (2026-11-07)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static bool A072698_test(uint64_t x, uint64_t *sum_out)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    if (cnt == 0) return false;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] != 1) return false;
    }
    uint64_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) sum += primes[i];
    if (sum % cnt != 0) return false;
    *sum_out = sum;
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next = 2;

    if (n == 0) return 0;

    while (len < (size_t)n) {
        uint64_t value = next++;
        uint64_t s = 0;
        if (A072698_test(value, &s)) {
            if (len >= cap) {
                size_t new_cap = (cap == 0) ? 16 : cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = (seq_t)s;
        }
    }
    return cache[(size_t)n - 1];
}
