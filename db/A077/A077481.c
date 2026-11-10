/*
 * A077481  Quotient A077480(n) / A077479(n).
 * https://oeis.org/A077481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(m)/omega(m), where m is the n-th integer >= 2
 *            with omega(m) dividing bigomega(m); found by a linear scan with
 *            caching
 * Range:     u8 n <= 7000, u16 n <= 7000, u32 n <= 7000, u64 n <= 7000,
 *            u128 n <= 7000, big any n
 * Validated: standard, pass, 121 terms through n = 6920 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *q_cache = NULL;
static size_t q_len = 0;
static size_t q_cap = 0;
static uint64_t q_next = 2;

static void q_scan_to(size_t target)
{
    while (q_len < target) {
        uint64_t m = q_next++;
        uint64_t x = m;
        unsigned big = 0, om = 0;
        for (uint64_t p = 2; p * p <= x; ++p) {
            if (x % p == 0) {
                ++om;
                while (x % p == 0) {
                    x /= p;
                    ++big;
                }
            }
        }
        if (x > 1) {
            ++om;
            ++big;
        }
        if (om && big % om == 0) {
            if (q_len == q_cap) {
                size_t nc = q_cap ? q_cap * 2 : 1024;
                uint32_t *t = (uint32_t *)realloc(q_cache, nc * sizeof(uint32_t));
                if (!t) abort();
                q_cache = t;
                q_cap = nc;
            }
            q_cache[q_len++] = big / om;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (q_len < (size_t)n) q_scan_to((size_t)n);
    return (seq_t)q_cache[n - 1];
}
