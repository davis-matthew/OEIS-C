/*
 * A043126  Numbers k such that 2 and 4 occur juxtaposed in the base-6
 *          representation of k but not of k-1.
 * https://oeis.org/A043126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043126: numbers k such that 2 and 4 occur juxtaposed in the
   base‑6 representation of k but not of k-1.  Computed by an incremental
   search that memoises found terms.  The function works for any
   seq_t/idx_t combination, including arbitrary‑precision seq_t. */

#include <stdlib.h>
#include <stdbool.h>

/* Return true if the base‑6 representation of x contains the digraph
   24 or 42.  Handles both integer and arbitrary‑precision seq_t. */
static bool has_adjacent_2_4(seq_t x)
{
    bool prev_set = false;
    seq_t prev = 0;

    while (x > 0)
    {
        seq_t d = x % 6;
        x = x / 6;

        if (prev_set && ((prev == 2 && d == 4) || (prev == 4 && d == 2)))
            return true;

        prev = d;
        prev_set = true;
    }
    return false;
}

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of the sequence values. */
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static seq_t last_checked = 0;   /* last k examined */

    if (n == 0)
        return 0;     /* Undefined for offset < 1 */

    /* Grow the cache until it holds at least n terms. */
    while (terms_len < n)
    {
        /* Resize the array if necessary. */
        if (terms_len == terms_cap)
        {
            size_t new_cap = (terms_cap == 0) ? 64 : terms_cap * 2;
            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();          /* Allocation failed */
            terms = tmp;
            terms_cap = new_cap;
        }

        /* Examine the next integer k. */
        seq_t k = last_checked + 1;
        last_checked = k;

        /* Check the condition. */
        if (has_adjacent_2_4(k) && !has_adjacent_2_4(k - 1))
        {
            terms[terms_len++] = k;
        }
    }

    return terms[n - 1];   /* Sequence is 1‑based. */
}
