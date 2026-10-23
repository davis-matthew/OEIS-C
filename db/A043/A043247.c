/*
 * A043247  Numbers k such that 4 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth positive integer k such that k contains
   the adjacent digits 4 and 6 (in either order) in base‑10 while
   k-1 does *not* contain such a pair.  Offset is 1. */

#include <stdlib.h>
#include <stdbool.h>

/* a minimal DP helper: does x contain digits 4&6 or 6&4 consecutively? */
static bool has_pair(seq_t x)
{
    /* sentinel for the previous digit */
    seq_t prev = 10;                 /* 10 is not a decimal digit */
    while (x > 0) {
        seq_t d = x % 10;
        if ((d == 4 && prev == 6) || (d == 6 && prev == 4))
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

/* Cache of already found terms */
static seq_t *seq_cache = NULL;   /* array of computed terms */
static idx_t  seq_len   = 0;       /* number of terms stored so far */
static idx_t  seq_cap   = 0;       /* allocated capacity of seq_cache */
static idx_t  last_checked = 0;    /* last integer examined */

/* Append a new value to the cache, expanding it if necessary. */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        seq_cap = seq_cap ? seq_cap * 2 : 128;
        /* reallocation needs an explicit cast in C++ */
        seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – terminate (no exception handling allowed) */
            exit(EXIT_FAILURE);
        }
        seq_cache = tmp;
    }
    seq_cache[seq_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* offset starts at 1; this case is unused */
        return 0;

    /* Grow the cache until we have at least n terms. */
    while (seq_len < n) {
        last_checked++;
        if (has_pair((seq_t)last_checked) && !has_pair((seq_t)(last_checked - 1)))
            add_term((seq_t)last_checked);
    }

    return seq_cache[n - 1];
}
