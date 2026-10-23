/*
 * A036994  Numbers k with the property that reading from right to left in the
 *          binary expansion of k, the number of 1's always stays ahead of the
 *          number of 0's.
 * https://oeis.org/A036994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the n-th positive integer k such that when reading the
 *            binary expansion of k from right to left, the running count of
 *            1's always stays strictly greater than the running count of 0's
 * Range:     u8 n <= 44, u16 n <= 7061, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;        /* stored sequence values    */
static idx_t cache_len = 0;        /* number of values          */
static idx_t cache_cap = 0;        /* capacity of cache array   */
static seq_t next_candidate = 1;   /* next odd integer to test  */

/* test whether a candidate satisfies the ballot property */
static inline int valid_seq(seq_t k)
{
    seq_t b = k;
    int diff = 0;                 /* count1 - count0          */
    while (b) {
        if (b & 1)
            diff++;
        else
            diff--;
        if (diff <= 0)
            return 0;
        b >>= 1;
    }
    return 1;
}

/* grow the cache array to at least 'needed' elements                */
static inline void grow_cache(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* return the next valid integer and advance the scanning pointer    */
static inline seq_t next_valid(void)
{
    while (1) {
        seq_t cand = next_candidate;   /* cand is always odd          */
        if (valid_seq(cand)) {
            next_candidate += 2;        /* move to following odd       */
            return cand;
        }
        next_candidate += 2;            /* even ones are skipped       */
    }
}

/* external linkage function that returns the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* sequence starts at n = 1    */

    if (cache_len < n) {
        grow_cache(n);
        while (cache_len < n) {
            seq_t val = next_valid();
            cache[cache_len++] = val;
        }
    }
    return cache[n - 1];
}
