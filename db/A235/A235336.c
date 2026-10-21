/*
 * A235336  Numbers having evil number of 1's in their binary representation.
 * https://oeis.org/A235336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th integer whose binary weight has even binary
 *            weight
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcnt_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* e.g. a(1)=7 because popcount(7)=3 and 3 in binary has 2 ones (even). */

/* This implementation caches results. It scans integers sequentially
 * and keeps track of those that satisfy the condition.   */

/* The algorithm is fully generic – it works for uint8_t…uint64_t, __int128, or
 * an arbitrary‑precision integer class that overloads basic arithmetic
 * operators and bitwise shifts.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/popcnt_uint.h"

/*-- Generic popcount for seq_t -------------------------------------------*/
static inline unsigned popcnt_seq(seq_t x)
{
    unsigned cnt = 0;
    while (x) {
        cnt += (unsigned)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/*-- test whether a seq_t value satisfies the OEIS condition ------------*/
static inline bool is_evil_popcount(seq_t x)
{
    unsigned pc = popcnt_seq(x);        /* popcount of x  */
    unsigned pc_of_pc = popcnt_uint(pc);/* popcount of pc  */
    return (pc_of_pc % 2) == 0;         /* even popcount -> evil */
}

/*-- main sequence function -----------------------------------------------*/
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 0;

    /* grow cache to hold at least n+1 elements */
    while (cache_len <= (size_t)n) {
        seq_t cand = next_candidate;
        if (is_evil_popcount(cand)) {
            /* ensure capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
        next_candidate++;
    }
    return cache[(size_t)n];
}
