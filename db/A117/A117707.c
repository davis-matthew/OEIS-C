/*
 * A117707  Largest number of previous terms having at least one common digit
 *          in decimal representation.
 * https://oeis.org/A117707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117707: Largest number of previous terms having at least one common digit. 
   a(0) = 0; for n>0, a(n) = max_{d=0..9} #{k < n : a(k) contains digit d}. */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached sequence values   */
    static idx_t cache_len = 0;     /* number of values stored  */
    static idx_t cache_cap = 0;     /* allocated capacity       */
    static size_t counts[10] = {0}; /* how many previous terms have digit d */

    /* Grow the cache as needed.  Use a cast that works for C and C++. */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap ? (cache_cap << 1) : 1;
        while (new_cap <= n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) abort();          /* memory failure   */
        cache_cap = new_cap;
    }

    /* Return a known value immediately. */
    if (n < cache_len)
        return cache[(size_t)n];

    /* Compute terms up to and including n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) is the largest count among the digits. */
        size_t maxc = 0;
        for (int d = 0; d < 10; ++d)
            if (counts[d] > maxc) maxc = counts[d];
        seq_t val = (seq_t)maxc;
        cache[(size_t)i] = val;

        /* Update digit counts for the new term.  Count each distinct digit once. */
        unsigned mask = 0;
        if (val == (seq_t)0) {
            mask = 1U << 0;
        } else {
            seq_t tmp = val;
            while (tmp != (seq_t)0) {
                unsigned d = (unsigned)(tmp % (seq_t)10);
                mask |= 1U << d;
                tmp /= (seq_t)10;
            }
        }
        for (int d = 0; d < 10; ++d)
            if (mask & (1U << d)) ++counts[d];
    }

    cache_len = n + 1;
    return cache[(size_t)n];
}
