/*
 * A043622  Numbers whose base-7 representation has exactly 7 runs.
 * https://oeis.org/A043622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Goal:  a(n) = the n‑th integer whose base‑7 representation has exactly 7 runs
 * Run = consecutive block of equal digits.
 * The sequence starts with 120100,120101,… as given in OEIS A043622.
 * We compute terms on demand and cache them for future calls.
 */

#include <stdlib.h>

/* Count the number of runs in the base‑7 representation of x. */
static unsigned count_runs(seq_t x)
{
    unsigned runs = 0;
    int last = -1;                     /* no previous digit, sentinel value */
    while (x != (seq_t)0)
    {
        int d = (int)(x % 7);          /* current base‑7 digit */
        if (d != last)                /* start of a new run */
        {
            runs++;
            last = d;
        }
        x = x / 7;                     /* move to next digit */
    }
    return runs;
}

/* Cached terms of the sequence. */
static seq_t *cache       = NULL;     /* array of found terms */
static size_t cache_len   = 0;         /* number of terms stored */
static size_t cache_cap   = 0;         /* allocated capacity */
static seq_t next_candidate = 1;      /* next integer to test */

/* Return the n-th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)          /* OEIS offset is 1; return 0 for safety */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Find the next integer with exactly 7 runs. */
        while (count_runs(next_candidate) != 7)
            next_candidate++;

        /* Allocate more space if needed. */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = next_candidate;
        cache_len++;
        next_candidate++;   /* ensure strictly increasing candidates */
    }

    return cache[(size_t)n - 1];
}
