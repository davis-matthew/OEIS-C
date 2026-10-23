/*
 * A192336  Sums of two or more distinct squares.
 * https://oeis.org/A192336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 37 (for n >= 92)
 * Range:     u8 n <= 218, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* helper: popcount for masks up to 14 bits */
static int popcnt(uint16_t x)
{
    int c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

/* comparator for qsort on uint64_t */
static int cmp_uint64(const void *p, const void *q)
{
    uint64_t a = *(const uint64_t *)p;
    uint64_t b = *(const uint64_t *)q;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static bool ready = false;
    static seq_t seq[200];   /* enough to hold all terms up to n = 91 */
    static size_t len = 0;

    if (!ready)
    {
        /* squares up to 14^2 = 196, enough to generate all sums <= 200 */
        enum { MAX_K = 14 };        /* 1..14 */
        enum { LIMIT = 200 };       /* sums considered */
        uint64_t squares[MAX_K];
        for (int i = 1; i <= MAX_K; ++i)
            squares[i - 1] = (uint64_t)i * (uint64_t)i;

        enum { MAX_MASK = 1 << MAX_K };   /* 16384 */
        uint64_t sums[MAX_MASK];
        size_t sum_cnt = 0;

        for (uint16_t mask = 0; mask < MAX_MASK; ++mask)
        {
            if (popcnt(mask) < 2) continue;   /* need at least two distinct squares */
            uint64_t sum = 0;
            for (int i = 0; i < MAX_K; ++i)
                if (mask & (1u << i))
                    sum += squares[i];
            if (sum <= LIMIT)
                sums[sum_cnt++] = sum;
        }

        /* sort and deduplicate */
        qsort(sums, sum_cnt, sizeof(uint64_t), cmp_uint64);
        size_t cur = 0;
        uint64_t prev = 0;
        bool first = true;
        for (size_t i = 0; i < sum_cnt && cur < sizeof(seq) / sizeof(seq_t); ++i)
        {
            uint64_t val = sums[i];
            if (first || val != prev)
            {
                seq[cur++] = (seq_t)val;
                prev = val;
                first = false;
            }
        }
        len = cur;
        ready = true;
    }

    if (n <= len)
        return seq[n - 1];

    /* for n >= 92 the formula a(n) = n + 37 holds */
    return (seq_t)n + (seq_t)37;
}
