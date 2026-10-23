/*
 * A043718  Numbers whose base-15 representation has an odd number of runs.
 * https://oeis.org/A043718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth number whose base‑15 representation has an odd number of runs
   by scanning numbers and counting run transitions.  The sequence is
   A043718. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test if a base‑15 number has an odd number of runs. */
static bool odd_runs(seq_t x)
{
    if (x == (seq_t)0)
        return true;                 /* not used, but safe for 0 */

    seq_t digit = x % (seq_t)15;      /* first (least significant) digit */
    int runs   = 1;                   /* start with one run */
    x /= (seq_t)15;

    while (x > (seq_t)0) {
        seq_t nd = x % (seq_t)15;
        if (nd != digit) {
            runs++;
            digit = nd;               /* new run starts */
        }
        x /= (seq_t)15;
    }
    return (runs & 1) != 0;           /* true if runs is odd */
}

/* Static cache of computed sequence values. */
static seq_t *cache   = NULL;   /* pointer to array of seq_t */
static size_t cache_cap   = 0;   /* capacity of the array */
static size_t cache_len   = 0;   /* number of elements stored */

/* Ensure the cache has at least *cap* slots.  Grow exponentially. */
static void ensure_cache_cap(size_t cap)
{
    if (cache_cap >= cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < cap)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence function.  External linkage as requested. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)          /* offset starts at 1; 0 is unused */
        return (seq_t)0;

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Start scanning from the next candidate after the last cached value. */
    seq_t candidate = cache_len == 0 ? (seq_t)1 : cache[cache_len - 1] + (seq_t)1;

    while (cache_len < (size_t)n) {
        if (odd_runs(candidate)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len] = candidate;
            cache_len++;
        }
        candidate++;
    }

    return cache[(size_t)n - 1];
}
