/*
 * A072699  Number of prime factors of A072697(n).
 * https://oeis.org/A072699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(A072697(n)); A072697 = squarefree k with (sum of
 *            prime factors of k) divisible by omega(k). Computed by a cached
 *            linear search
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9706 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static unsigned A072699_test(uint64_t x)
{
    uint64_t sum = 0;
    unsigned cnt = 0;
    uint64_t t = x;
    if (t % 2 == 0) {
        t /= 2;
        if (t % 2 == 0) return 0;
        sum += 2;
        cnt++;
    }
    for (uint64_t d = 3; d <= t / d; d += 2) {
        if (t % d == 0) {
            t /= d;
            if (t % d == 0) return 0;
            sum += d;
            cnt++;
        }
    }
    if (t > 1) {
        sum += t;
        cnt++;
    }
    if (cnt == 0) return 0;
    if (sum % cnt != 0) return 0;
    return cnt;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static uint64_t next = 2;

    if (n == 0) return 0;
    size_t want = (size_t)n;

    while (len < want) {
        uint64_t v = next++;
        unsigned c = A072699_test(v);
        if (c) {
            if (len >= cap) {
                size_t nc = cap ? cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, nc);
                if (!tmp) abort();
                cache = tmp;
                cap = nc;
            }
            cache[len++] = (seq_t)c;
        }
    }
    return cache[want - 1];
}
