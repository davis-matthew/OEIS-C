/*
 * A002645  Quartan primes: primes of the form x^4 + y^4, x > 0, y > 0.
 * https://oeis.org/A002645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate x^4 + y^4 (1 <= x <= y) up to a growing
 *            bound, keep the primes (A000040 INTERSECTION A003336), sort them
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 3368, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 9384 (2026-11-03)
 * Helpers:   is_prime_u64, mulmod, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_u64.h"
#include "../utils/mulmod.h"
#include "../utils/powmod_u64.h"

static int q_cmp(const void *p, const void *q) {
    uint64_t u = *(const uint64_t *)p, v = *(const uint64_t *)q;
    return (u > v) - (u < v);
}

static uint64_t *q_cache = NULL;
static size_t q_len = 0;
static uint64_t q_limit = 0;

static void q_build(uint64_t limit) {
    size_t cap = 1024, len = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    uint64_t x, y;
    for (x = 1; x * x * x * x < limit; x++) {
        uint64_t x4 = x * x * x * x;
        for (y = x; ; y++) {
            uint64_t s = x4 + y * y * y * y;
            if (s > limit) break;
            if (((x + y) & 1) == 0 && !(x == 1 && y == 1)) continue;
            if (is_prime_u64(s)) {
                if (len == cap) {
                    cap *= 2;
                    v = (uint64_t *)realloc(v, cap * sizeof(uint64_t));
                }
                v[len++] = s;
            }
        }
    }
    qsort(v, len, sizeof(uint64_t), q_cmp);
    free(q_cache);
    q_cache = v;
    q_len = len;
    q_limit = limit;
}

seq_t a(idx_t n) {
    uint64_t limit = q_limit ? q_limit : ((uint64_t)1 << 20);
    if (q_limit == 0) q_build(limit);
    while ((idx_t)q_len < n) {
        limit *= 4;
        q_build(limit);
    }
    return (seq_t)q_cache[n - 1];
}
