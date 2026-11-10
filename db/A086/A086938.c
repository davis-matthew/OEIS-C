/*
 * A086938  Smallest m with A086571(m) = n.
 * https://oeis.org/A086938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest m with A086571(m) = n; found by generating
 *            A086571 (a(1)=1, a(m) = smallest divisor of m that occurred at
 *            most once earlier) and recording the first index of each value
 * Range:     u8 n <= 59, u16 n <= 7199, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9914 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint32_t *A086938_freq = NULL;
static uint32_t *A086938_first = NULL;
static size_t A086938_cap = 0;
static size_t A086938_done = 0;

static void A086938_grow(size_t cap)
{
    if (cap <= A086938_cap) return;
    uint32_t *f = (uint32_t *)realloc(A086938_freq, (cap + 1) * sizeof(uint32_t));
    if (!f) abort();
    A086938_freq = f;
    uint32_t *g = (uint32_t *)realloc(A086938_first, (cap + 1) * sizeof(uint32_t));
    if (!g) abort();
    A086938_first = g;
    for (size_t i = (A086938_cap == 0 ? 0 : A086938_cap + 1); i <= cap; ++i) {
        A086938_freq[i] = 0;
        A086938_first[i] = 0;
    }
    A086938_cap = cap;
}

static void A086938_extend(size_t limit)
{
    A086938_grow(limit);
    for (size_t m = A086938_done + 1; m <= limit; ++m) {
        size_t best = m;
        for (size_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                if (A086938_freq[d] <= 1 && d < best) best = d;
                size_t e = m / d;
                if (A086938_freq[e] <= 1 && e < best) best = e;
                if (best == 1) break;
            }
        }
        if (m == 1) best = 1;
        A086938_freq[best]++;
        if (A086938_first[best] == 0) A086938_first[best] = (uint32_t)m;
    }
    if (limit > A086938_done) A086938_done = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t nn = (size_t)n;
    if (A086938_cap < nn) A086938_grow(nn);
    size_t limit = A086938_done;
    if (limit < 64 * nn + 64) limit = 64 * nn + 64;
    if (A086938_first[nn] == 0 || limit > A086938_done) {
        if (A086938_first[nn] == 0) A086938_extend(limit);
    }
    while (A086938_first[nn] == 0) {
        limit = A086938_done * 2;
        A086938_extend(limit);
    }
    return (seq_t)A086938_first[nn];
}
