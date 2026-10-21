/*
 * A224810  Subsets of {1,2,...,n-6} without differences equal to 3 or 6.
 * https://oeis.org/A224810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 32, u32 n <= 61, u64 n <= 119,
 *            u128 n <= 235, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224810: Subsets of {1,2,…,n‑6} without differences equal to 3 or 6
 * Recurrence (OEIS formula):
 *   a(n) = a(n-1) - a(n-3) + 2*a(n-4) - 2*a(n-6) + 4*a(n-7)
 *          + 2*a(n-9) + 2*a(n-10) + 4*a(n-12) - 2*a(n-13)
 *          + 2*a(n-15) - 4*a(n-16) - 2*a(n-18) - 2*a(n-19)
 *          - a(n-21) - a(n-22) - a(n-24)
 * Initial values (a(0)…a(23)):
 *   1,1,1,1,1,1,1,2,4,8,12,18,27,36,48,64,96,144,216,324,486,729,1053,1521
 * The function memoises the values so later calls (in any order) are O(1).
 */

#include <stdlib.h>    /* malloc, realloc */

static seq_t *cache = NULL;          /* memoised values */
static size_t cache_len = 0;         /* current cache length */

static const seq_t init[24] = {
    (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
    (seq_t)2, (seq_t)4, (seq_t)8, (seq_t)12, (seq_t)18, (seq_t)27, (seq_t)36,
    (seq_t)48, (seq_t)64, (seq_t)96, (seq_t)144, (seq_t)216, (seq_t)324,
    (seq_t)486, (seq_t)729, (seq_t)1053, (seq_t)1521
};

seq_t a(idx_t n)
{
    /* Convert the supplied uint64_t index to size_t (non‑negative). */
    size_t idx = (size_t)n;

    /* Ensure the cache is large enough for idx. */
    if (idx >= cache_len) {
        size_t newlen = idx + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newlen * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: return zero (default constructor). */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute any newly required terms. */
        for (size_t i = cache_len; i < newlen; ++i) {
            if (i < 24) {
                cache[i] = init[i];
            } else {
                seq_t val = cache[i-1];
                val -= cache[i-3];
                val += (seq_t)2 * cache[i-4];
                val -= (seq_t)2 * cache[i-6];
                val += (seq_t)4 * cache[i-7];
                val += (seq_t)2 * cache[i-9];
                val += (seq_t)2 * cache[i-10];
                val += (seq_t)4 * cache[i-12];
                val -= (seq_t)2 * cache[i-13];
                val += (seq_t)2 * cache[i-15];
                val -= (seq_t)4 * cache[i-16];
                val -= (seq_t)2 * cache[i-18];
                val -= (seq_t)2 * cache[i-19];
                val -= cache[i-21];
                val -= cache[i-22];
                val -= cache[i-24];
                cache[i] = val;
            }
        }
        cache_len = newlen;
    }

    return cache[idx];
}
