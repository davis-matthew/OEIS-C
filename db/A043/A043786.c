/*
 * A043786  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 6.
 * https://oeis.org/A043786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043786: Numbers n whose base‑3 representation contains a number of runs
   congruent to 1 modulo 6.  The function searches for qualifying numbers
   and memoises results for efficient repeated access. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true when n has a number of runs in base‑3 ≡ 1 (mod 6). */
static inline bool is_qualifying(seq_t n)
{
    if (n == (seq_t)0)
        return false;
    int runs = 0;
    int prev = -1;
    seq_t t = n;
    while (t > (seq_t)0) {
        int d = (int)(t % (seq_t)3);   /* d is 0, 1, or 2 */
        if (d != prev) {
            ++runs;
            prev = d;
        }
        t /= (seq_t)3;
    }
    return (runs % 6) == 1;
}

/* Compute the n-th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t next_candidate = (seq_t)1;   /* next integer to test */

    if (n == 0)              /* offset is 1; behaviour for n<1 is irrelevant */
        return (seq_t)0;

    /* Extend the cache until we have at least n terms. */
    while (cache_len < n) {
        /* Find the next qualifying number. */
        while (1) {
            if (is_qualifying(next_candidate)) {
                /* Grow the cache if necessary. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                    if (new_cap < cache_len + 1)
                        new_cap = cache_len + 1;
                    seq_t *new_buf = mem_new(seq_t, new_cap);
                    if (!new_buf) abort();            /* allocation failed */
                    if (cache) {
                        mem_copy(new_buf, cache, cache_len * sizeof(seq_t));
                        mem_free(cache);
                    }
                    cache = new_buf;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = next_candidate;
                ++next_candidate;                /* next number to test */
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
