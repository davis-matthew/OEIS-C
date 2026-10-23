/*
 * A043961  Numbers k such that 0 and 2 occur juxtaposed in the base-9
 *          representation of k but not of k+1.
 * https://oeis.org/A043961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that the base‑9 representation of k
   contains the adjacent digits 0 and 2 (in either order) but
   the base‑9 representation of k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>

/* Return true iff x has adjacent 0 and 2 in its base‑9 representation. */
static bool has_adjacent_0_2(seq_t x)
{
    if (x == 0) {
        return false;
    }

    seq_t cur = x;
    int prev = -1;          /* previous digit (from least significant) */

    while (cur > 0) {
        int d = (int)(cur % 9);       /* 0 <= d <= 8, fits in an int */
        if ((prev == 0 && d == 2) || (prev == 2 && d == 0)) {
            return true;
        }
        prev = d;
        cur /= 9;
    }
    return false;
}

/* Return the n‑th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static size_t   len   = 0;
    static size_t   cap   = 0;
    static seq_t   next_k = 0;   /* smallest k not yet examined */

    /* initialise cache */
    if (cache == NULL) {
        cap = 64;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* fill cache until we have at least n elements */
    while (len < n) {
        if (has_adjacent_0_2(next_k) && !has_adjacent_0_2(next_k + 1)) {
            if (len == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = next_k;
        }
        ++next_k;   /* next integer to test */
    }

    return cache[n - 1];
}
