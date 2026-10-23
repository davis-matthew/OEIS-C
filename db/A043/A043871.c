/*
 * A043871  Number of runs in the base 4 representation of n is congruent to 4
 *          mod 10.
 * https://oeis.org/A043871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth integer whose base‑4 representation has
   a number of runs congruent to 4 (mod 10).  The runs are counted as
   consecutive blocks of identical digits in the usual base‑4 writing
   without leading zeros. */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;          /* Array of found numbers */
static size_t cache_cap = 0;         /* Allocated capacity */
static idx_t cache_used = 0;         /* Number of entries stored */
static seq_t last_candidate = 0;     /* Last integer tested */

/* Extend the cache until it contains at least 'n' entries. */
static void extend_cache_to(idx_t n)
{
    while (cache_used < n) {
        ++last_candidate;

        /* Count runs in the base-4 representation of last_candidate */
        seq_t t = last_candidate;
        seq_t prev_digit = 4;        /* sentinel not equal to 0..3 */
        int runs = 0;
        while (t > 0) {
            seq_t digit = t % 4;
            if (digit != prev_digit) {
                ++runs;
                prev_digit = digit;
            }
            t /= 4;
        }

        if (runs % 10 == 4) {        /* Record numbers satisfying the property */
            if (cache_used == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_used++] = last_candidate;
        }
    }
}

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* Undefined for offset 1, but harmless */
    extend_cache_to(n);
    return cache[n - 1];
}
