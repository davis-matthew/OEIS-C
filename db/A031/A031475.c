/*
 * A031475  Numbers whose base-5 representation has 2 fewer 0's than 4's.
 * https://oeis.org/A031475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers whose base‑5 representation has 2 fewer 0’s than 4’s. No
 *            closed formula; we generate terms in order
 * Range:     u8 n <= 19, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static cache holding generated terms (1‑based indexing) */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
static idx_t  cache_sz  = 0;
static seq_t  next_candidate = 1;

/* Check whether a value satisfies the digit condition. */
static bool satisfies(seq_t x)
{
    int count0 = 0;
    int count4 = 0;
    while (x > 0) {
        int d = (int)(x % 5);
        if (d == 0)            count0++;
        else if (d == 4)       count4++;
        x /= 5;
    }
    return (count4 - count0 == 2);
}

/* Append a new term to the cache, resizing if necessary. */
static void append(seq_t val)
{
    if (cache_sz >= cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_sz++] = val;
}

/* External linkage: sequence function. */
seq_t a(idx_t n)
{
    while (cache_sz < n) {
        if (satisfies(next_candidate))
            append(next_candidate);
        next_candidate++;
    }
    return cache[n - 1];
}
