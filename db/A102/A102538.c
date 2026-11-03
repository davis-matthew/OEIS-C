/*
 * A102538  Numbers m not of the form k*(k+2) that have a single '1' in the
 *          periodic part of the continued fraction of sqrt(n).
 * https://oeis.org/A102538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 999 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* m is a term iff m is not a square, m + 1 is not a square (m != k(k+2)),
   and exactly one partial quotient in the period of the continued fraction
   of sqrt(m) is 1. The standard recurrence m' = d a - m, d' = (D - m'^2)/d,
   a' = (a0 + m')/d' runs until a' = 2 a0 ends the period; all in 64 bits (in
   seq_t it timed out under big), with the terms cached. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static int period_one_count(uint64_t D, uint64_t a0)
{
    uint64_t m = 0, d = 1, a = a0;
    int cnt = 0;
    for (;;) {
        uint64_t mn = d * a - m;
        uint64_t dn = (D - mn * mn) / d;
        uint64_t an = (a0 + mn) / dn;
        if (an == 1) ++cnt;
        if (an == 2 * a0) break;              /* end of period */
        m = mn;
        d = dn;
        a = an;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t size = 0, cap = 0;
    static uint64_t last = 0, r = 0;          /* r = isqrt(last + 1), kept as last grows */
    if (n == 0) return (seq_t)0;
    while (size < (size_t)n) {
        uint64_t m = ++last;
        while ((r + 1) * (r + 1) <= m) ++r;   /* r = isqrt(m) */
        if (r * r == m || (r + 1) * (r + 1) == m + 1) continue;
        if (period_one_count(m, r) != 1) continue;
        if (size == cap) {
            cap = cap ? 2 * cap : 64;
            seq_t *t = mem_resize(seq_t, cache, cap);
            if (!t) abort();
            cache = t;
        }
        cache[size++] = (seq_t)m;
    }
    return cache[n - 1];
}
