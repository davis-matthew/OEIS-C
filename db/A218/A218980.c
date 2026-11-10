/*
 * A218980  Integers not in A218852.
 * https://oeis.org/A218980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer m that is NOT of the form
 *            sigma(m)=sigma(x)+sigma(y)+sigma(z) with m=x+y+z, x,y,z>0 (sieve
 *            + search)
 * Range:     u8 n <= 56, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *A218980_sig = NULL;
static uint64_t A218980_lim = 0;
static uint64_t *A218980_terms = NULL;
static uint64_t A218980_cnt = 0, A218980_cap = 0;
static uint64_t A218980_next = 1;

static void A218980_grow(uint64_t limit)
{
    uint64_t *s = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!s) return;
    for (uint64_t i = 0; i <= limit; ++i) s[i] = 0;
    for (uint64_t i = 1; i <= limit; ++i)
        for (uint64_t j = i; j <= limit; j += i) s[j] += i;
    free(A218980_sig);
    A218980_sig = s;
    A218980_lim = limit;
}

static bool A218980_in_218852(uint64_t m)
{
    if (m < 3) return false;
    const uint64_t *s = A218980_sig;
    uint64_t sm = s[m];
    for (uint64_t x = 1; x + 2 <= m; ++x) {
        if (3 * x > m) break;
        uint64_t sx = s[x];
        if (sx > sm) continue;
        uint64_t target = sm - sx;
        uint64_t rem = m - x;
        for (uint64_t y = x; 2 * y <= rem; ++y) {
            uint64_t z = rem - y;
            if (s[y] + s[z] == target) return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (A218980_sig == NULL) A218980_grow(4096);
    while (A218980_cnt < (uint64_t)n) {
        uint64_t m = A218980_next++;
        if (m > A218980_lim) A218980_grow(m * 2);
        if (!A218980_in_218852(m)) {
            if (A218980_cnt + 1 > A218980_cap) {
                uint64_t nc = A218980_cap ? A218980_cap * 2 : 64;
                uint64_t *t = (uint64_t *)realloc(A218980_terms, nc * sizeof(uint64_t));
                if (!t) return 0;
                A218980_terms = t;
                A218980_cap = nc;
            }
            A218980_terms[A218980_cnt++] = m;
        }
    }
    return (seq_t)A218980_terms[n - 1];
}
