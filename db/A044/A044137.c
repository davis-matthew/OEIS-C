/*
 * A044137  Numbers k such that string 5,5 occurs in the base 6 representation
 *          of k but not of k-1.
 * https://oeis.org/A044137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1700, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044137: Numbers k such that the base‑6 representation of k
   contains the substring "55" while the base‑6 representation of k‑1
   does not.  The code searches numbers sequentially and caches the
   results to allow repeated calls with arbitrary indices. */

#include <stdbool.h>
#include <stdlib.h>

/* Check whether the base‑6 representation of x contains "55". */
static bool has_55(seq_t x)
{
    int last = -1;               /* previous least‑significant digit */
    while (x > 0) {
        int digit = (int)(x % 6);
        if (last == 5 && digit == 5) return true;
        last = digit;
        x /= 6;
    }
    return false;                /* empty or no “55” found */
}

/* Cache of found terms. */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;   /* number of terms currently cached */
static size_t cache_cap  = 0;   /* allocated capacity of cache */
static seq_t next_k      = 1;   /* next integer to test */

/* Add a term to the cache, growing the array if necessary. */
static void add_term(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!newp) exit(1);     /* memory exhausted – abort */
        cache  = newp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n-th term of A044137 (1‑based index). */
seq_t a(idx_t n)
{
    /* Ensure we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        if (has_55(next_k) && !has_55(next_k - 1))
            add_term(next_k);
        next_k++;
    }
    return cache[n - 1];
}
