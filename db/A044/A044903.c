/*
 * A044903  Base-2 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044903: a(n) is the nth positive integer whose binary run
   lengths alternate odd, even, odd, … starting with odd.  The
   implementation keeps a cache and checks each candidate for the
   alternating‑run property. */

#include <stdbool.h>
#include <stddef.h>
#ifdef __cplusplus
#  include <vector>
#endif

/* Compute the bit length of a positive integer of type seq_t.
   Works for any unsigned integer type or arbitrary-precision class. */
static inline unsigned bitlen_seq_t(seq_t x)
{
    unsigned len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Test whether the binary representation of v has run lengths that
   alternately satisfy odd / even / odd / ... starting with odd. */
static bool alternating_runs(seq_t v)
{
    if (v == 0)
        return false;                     /* 0 is not positive */

    bool needOdd  = true;
    unsigned len  = bitlen_seq_t(v);
    int    pos    = (int)len - 1;         /* start at MSB */

    while (pos >= 0) {
        bool bit = ((v >> pos) & 1) != 0;
        unsigned run = 0;
        while (pos >= 0 && ((v >> pos) & 1) == bit) {
            ++run;
            --pos;
        }
        if ((run & 1) != needOdd)         /* parity mismatch */
            return false;
        needOdd = !needOdd;                /* alternate expectation */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* In C++ mode use a vector for automatic memory management.
       In C mode use a malloc/realloc pair. */
#if defined(__cplusplus)

    static std::vector<seq_t> cache;

    if (n <= cache.size())
        return cache[n - 1];

    /* Search for successive terms until the requested index is reached. */
    seq_t candidate = cache.empty() ? 1 : cache.back() + 1;
    while (cache.size() < n) {
        if (alternating_runs(candidate))
            cache.push_back(candidate);
        ++candidate;
    }
    return cache[n - 1];

#else   /* C mode */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if (n <= cache_len)
        return cache[n - 1];

    /* Ensure the cache is large enough. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (new_cap < (size_t)n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                      /* out of memory – unlikely */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for successive terms until the requested index is reached. */
    seq_t candidate = (cache_len == 0) ? 1 : cache[cache_len - 1] + 1;
    while (cache_len < (size_t)n) {
        if (alternating_runs(candidate))
            cache[cache_len++] = candidate;
        ++candidate;
    }
    return cache[n - 1];

#endif
}
