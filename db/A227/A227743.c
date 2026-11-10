/*
 * A227743  Integers n for which A173318(n) is a square.
 * https://oeis.org/A227743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th integer m >= 0 such that A173318(m) is a square,
 *            where A173318(m) = Sum_{i=0..m} popcount(i XOR (i>>1)); found by
 *            a running scan with memoization
 * Range:     u8 n <= 17, u16 n <= 158, u32 n <= 937, u64 n <= 937,
 *            u128 n <= 937, big any n
 * Validated: standard, pass, 66 terms through n = 909 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static int sq_u64_227743(uint64_t v)
{
    uint64_t r = (uint64_t)sqrt((double)v);
    while (r * r > v) r--;
    while ((r + 1) * (r + 1) <= v) r++;
    return r * r == v;
}

seq_t a(idx_t n)
{
    static uint64_t *found = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t next_m = 0;   /* next index to process */
    static uint64_t sum = 0;      /* partial sum through next_m - 1 */

    if (n < 1) return (seq_t)0;
    while (cnt < (size_t)n) {
        uint64_t m = next_m++;
        sum += (uint64_t)__builtin_popcountll(m ^ (m >> 1));
        if (sq_u64_227743(sum)) {
            if (cnt == cap) {
                size_t nc = cap ? cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(found, nc * sizeof(uint64_t));
                if (!p) return (seq_t)0;
                found = p;
                cap = nc;
            }
            found[cnt++] = m;
        }
    }
    return (seq_t)found[n - 1];
}
