/*
 * A039348  Numbers whose base-8 representation has the same nonzero number of
 *          3's and 5's.
 * https://oeis.org/A039348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A039348.
 * a(n) is the nth positive integer whose base‑8 representation
 * contains the same non‑zero number of digits 3 and 5.
 * The routine performs a simple linear search, remembering all
 * previously found terms in a dynamic array so that further calls
 * are very fast regardless of the order of the indices. */

#include <stdlib.h>
#include <stdbool.h>

/* internal helper: test whether a number has at least one 3 and one 5
 * and those counts are equal.  The base‑8 digits are obtained by
 * repeated division and modulo by 8. */
static inline bool has_equal_nonzero_35(seq_t x)
{
    seq_t t = x;
    int c3 = 0, c5 = 0;
    while (t > 0) {
        unsigned d = (unsigned)(t % (seq_t)8);
        if (d == 3)
            ++c3;
        else if (d == 5)
            ++c5;
        t /= (seq_t)8;
    }
    return (c3 == c5) && (c3 > 0);
}

/* public API: return the nth term (n >= 1).  The function memoises
 * all previously computed terms, so calls are order‑independent. */
seq_t a(idx_t n)
{
    /* static cache of terms already found */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* next integer to test */
    static seq_t next_to_check = 1;

    /* grow the cache if need be */
    if (cache_cap == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* keep generating terms until we have at least n of them */
    while (cache_size < n) {
        if (has_equal_nonzero_35(next_to_check)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = next_to_check;
        }
        ++next_to_check;
    }

    return cache[n - 1];
}
