/*
 * A043749  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 3 mod 8.
 * https://oeis.org/A043749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n whose binary representation has a number of runs
 * congruent to 3 modulo 8.  Computed by scanning integers and storing
 * those that satisfy the condition. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* array of found terms */
static size_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap  = 0;     /* allocated capacity */
static seq_t next_candidate = 1;  /* integer to test next */

/* Count runs in the binary expansion of a 64‑bit value. */
static inline int count_runs_uint64(uint64_t v)
{
    int runs = 0;
    int prev = -1;             /* none yet */
    while (v) {
        int bit = v & 1;
        if (bit != prev) {
            ++runs;
            prev = bit;
        }
        v >>= 1;
    }
    return runs;
}

seq_t a(idx_t n)
{
    /* Build terms up to the requested index if necessary */
    while (cache_size < (size_t)n) {
        seq_t cand = next_candidate++;
        uint64_t x = (uint64_t)cand;          /* safe: the required terms are far below 2⁶⁴ */
        if (count_runs_uint64(x) % 8 == 3) {
            /* Ensure there is room for the new element */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }
    }
    return cache[n - 1];
}
