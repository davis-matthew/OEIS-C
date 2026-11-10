/*
 * A238248  Numbers m such that A072219(m) = 7.
 * https://oeis.org/A238248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (s(n)+1)/2, where s(n) is the n-th odd number whose
 *            binary expansion has exactly 7 runs (equivalently, numbers m
 *            with 2m-1 having 7 binary runs); computed by scanning with a
 *            cache
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9928 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static uint64_t *c238248_cache = NULL;
static size_t c238248_len = 0;
static size_t c238248_cap = 0;
static uint64_t c238248_last = 0;

static int c238248_runs(uint64_t s)
{
    uint64_t t = s ^ (s >> 1);
    int c = 0;
    while (t) {
        c += (int)(t & 1);
        t >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (c238248_len < (size_t)n) {
        ++c238248_last;
        uint64_t s = 2 * c238248_last - 1;
        if (c238248_runs(s) == 7) {
            if (c238248_len == c238248_cap) {
                size_t nc = c238248_cap ? c238248_cap * 2 : 64;
                c238248_cache = (uint64_t *)realloc(c238248_cache, nc * sizeof(uint64_t));
                c238248_cap = nc;
            }
            c238248_cache[c238248_len++] = c238248_last;
        }
    }
    return (seq_t)c238248_cache[(size_t)n - 1];
}
