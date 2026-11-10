/*
 * A240832  Sequence R(n) arising from analysis of structure of A240830.
 * https://oeis.org/A240832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   R(n) = n - A240830(n-1), with A240830(0) = 1 (so R(1) = 0).
 *            A240830: a(n) = 1 for n <= 7; a(n) = Sum_{i=0..6}
 *            a(n-i-a(n-i-1)) for n > 7
 * Range:     u8 n <= 1318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9586 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A240830_buf = NULL;
static uint64_t A240830_cap = 0;
static uint64_t A240830_max = 0;

static uint64_t A240830_get(uint64_t n)
{
    if (n + 2 > A240830_cap) {
        uint64_t nc = A240830_cap ? A240830_cap * 2 : 64;
        while (nc < n + 2) nc *= 2;
        uint64_t *t = (uint64_t *)realloc(A240830_buf, nc * sizeof(uint64_t));
        if (!t) return 0;
        A240830_buf = t;
        A240830_cap = nc;
        if (A240830_max == 0) {
            A240830_buf[0] = 1;
        }
    }
    if (A240830_max == 0) {
        A240830_buf[0] = 1;
    }
    for (uint64_t i = A240830_max + 1; i <= n; ++i) {
        if (i <= 7) {
            A240830_buf[i] = 1;
        } else {
            uint64_t s = 0;
            for (uint64_t j = 0; j <= 6; ++j) {
                uint64_t idx = i - j - A240830_buf[i - j - 1];
                s += A240830_buf[idx];
            }
            A240830_buf[i] = s;
        }
    }
    if (n > A240830_max) A240830_max = n;
    return A240830_buf[n];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t prev = (n == 1) ? 1 : A240830_get((uint64_t)n - 1);
    return (seq_t)((uint64_t)n - prev);
}
