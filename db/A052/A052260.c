/*
 * A052260  Last filtering prime (A052180) of primes p such that next prime is
 *          p+16 (A031934).
 * https://oeis.org/A052260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max over composites j in (p, p+16) of
 *            least_prime_factor(j), where p is the n-th prime with
 *            nextprime(p) = p+16 (A031934); found by a prime scan with trial
 *            division
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t *g_vals = NULL;
static size_t g_cnt = 0, g_cap = 0;
static uint64_t g_p = 2; /* last prime examined */

static uint64_t A052260_lpf(uint64_t m)
{
    if ((m & 1) == 0) return 2;
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return d;
    return m;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (g_cnt < (size_t)n) {
        uint64_t p = g_p;
        uint64_t q = (p == 2) ? 3 : p + 2;
        while (!is_prime_u64(q)) q += 2;
        g_p = q;
        if (q - p == 16) {
            uint64_t mx = 0;
            for (uint64_t j = p + 1; j < q; ++j) {
                uint64_t l = A052260_lpf(j);
                if (l != j && l > mx) mx = l;
            }
            if (g_cnt == g_cap) {
                g_cap = g_cap ? g_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc((void *)g_vals, g_cap * sizeof(uint64_t));
                if (!t) abort();
                g_vals = t;
            }
            g_vals[g_cnt++] = mx;
        }
    }
    return (seq_t)g_vals[n - 1];
}
