/*
 * A063559  Smallest k such that 3^k has exactly n 4's in its decimal
 *          representation.
 * https://oeis.org/A063559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063559: the smallest k >= 1 with exactly n digits 4 in 3^k. The powers 3^0,
 * 3^1, ... are built in base 10^9 and their 4s counted; the first k for each
 * count is recorded as the scan goes, and the scan resumes for larger n.
 * (The original stopped at a fixed bound and returned 0 past it.) */

#include <stdint.h>
#include <stdlib.h>

#define KMAX 2000000                      /* give up beyond 3^KMAX */

static uint32_t *L = NULL;                /* 3^k, little-endian base 10^9 */
static size_t len = 0, cap = 0;
static uint64_t k = 0;
static int64_t *first = NULL;             /* first[c]: least k with c fours, -1 if none yet */
static size_t nfirst = 0;

static unsigned fours(void)
{
    unsigned c = 0;
    for (size_t i = 0; i < len; ++i) {
        uint32_t v = L[i];
        int d = 0;
        for (; d < 9 && (v || i + 1 < len); ++d) { c += v % 10 == 4; v /= 10; }
    }
    return c;
}

static void record(unsigned c)
{
    if (c >= nfirst) {
        size_t m = nfirst ? nfirst : 64;
        while (m <= c) m *= 2;
        int64_t *t = (int64_t *)realloc(first, m * sizeof *t);
        if (!t) abort();
        for (size_t i = nfirst; i < m; ++i) t[i] = -1;
        first = t;
        nfirst = m;
    }
    if (first[c] < 0) first[c] = (int64_t)k;
}

seq_t a(idx_t n)
{
    if (!L) {
        cap = 16;
        L = (uint32_t *)malloc(cap * sizeof *L);
        if (!L) abort();
        L[0] = 1;                          /* 3^0; k counts from 1 */
        len = 1;
    }
    while (((size_t)n >= nfirst || first[n] < 0) && k < KMAX) {
        uint64_t c = 0;
        for (size_t i = 0; i < len; ++i) {
            c += (uint64_t)L[i] * 3;
            L[i] = (uint32_t)(c % 1000000000u);
            c /= 1000000000u;
        }
        if (c) {
            if (len == cap) {
                uint32_t *t = (uint32_t *)realloc(L, (cap *= 2) * sizeof *L);
                if (!t) abort();
                L = t;
            }
            L[len++] = (uint32_t)c;
        }
        ++k;
        record(fours());
    }
    return (size_t)n < nfirst && first[n] >= 0 ? (seq_t)first[n] : (seq_t)0;
}
