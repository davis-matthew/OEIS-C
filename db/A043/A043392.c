/*
 * A043392  Numbers having four 5's in base 6.
 * https://oeis.org/A043392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   four_fives_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: first n numbers whose base‑6 representation contains exactly
   four digits equal to 5. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/four_fives_base6.h"

/* Ensure that the dynamic array has room for at least need more elements. */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    size_t new_cap = (*cap > 0) ? *cap : 1;
    while (new_cap < *cap + need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        exit(EXIT_FAILURE); /* allocation failed */
    *arr = tmp;
    *cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but return 0 to be safe */

    /* Static state: array of found terms, its length, capacity, and the
       next integer to examine. */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t next = 0;

    /* Extend the cache until it contains at least n terms. */
    while (seq_len < n) {
        if (four_fives_base6(next)) {
            if (seq_len == seq_cap)
                grow_array(&seq, &seq_cap, 1);
            seq[seq_len++] = (seq_t)next;
        }
        ++next;
    }

    return seq[n - 1];
}
