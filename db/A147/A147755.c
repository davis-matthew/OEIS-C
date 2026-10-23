/*
 * A147755  a(n) = the largest integer that, when represented in binary,
 *          occurs at least twice as a substring in the binary representation
 *          of n.
 * https://oeis.org/A147755
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) is the largest integer whose binary representation *
 *            (without leading zeros) occurs at least twice as a substring of
 *            the * binary representation of n, overlapping allowed. The
 *            function * enumerates all substrings that start with a '1',
 *            counts their * occurrences, and returns the largest valu...
 * Range:     u8 n <= 1022, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of bits needed to represent n (no leading zero). */
static unsigned bitlen_idx(idx_t n)
{
    unsigned len = 0;
    idx_t t = n;
    while (t) { ++len; t >>= 1; }
    return len;
}

seq_t a(idx_t n)
{
    if (n < 3U) return (seq_t)0;      /* offset is 3 */

    unsigned L = bitlen_idx(n);
    uint64_t u = (uint64_t)n;

    struct { uint64_t val; unsigned cnt; } items[2080];
    unsigned m = 0;                    /* number of distinct substrings */

    for (unsigned i = 0; i < L; ++i)
    {
        /* Skip substrings that start with a leading zero – they cannot
         * represent a binary integer. */
        if ( ((u >> (L-1-i)) & 1ULL) == 0 ) continue;

        uint64_t val = 0;
        for (unsigned j = i; j < L; ++j)
        {
            val = (val << 1) | ((u >> (L-1-j)) & 1ULL);

            unsigned k;
            for (k = 0; k < m; ++k)
            {
                if (items[k].val == val) { ++items[k].cnt; break; }
            }
            if (k == m)                    /* new substring */
            {
                items[m].val = val;
                items[m].cnt = 1;
                ++m;
            }
        }
    }

    uint64_t best = 0;
    for (unsigned i = 0; i < m; ++i)
    {
        if (items[i].cnt >= 2 && items[i].val > best)
            best = items[i].val;
    }

    return (seq_t)best;
}
