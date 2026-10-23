/*
 * A043218  Numbers k such that 0 and 3 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent_03or30
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers k such that 0 and 3 occur juxtaposed (adjacent in any order)
    in the base‑10 representation of k but not of k-1.
    Computed by a simple brute‑force search up to the requested index. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent_03or30.h"

/* static state for memoization */
static seq_t *terms = NULL;
static size_t term_capacity = 0;
static idx_t term_count = 0;
static seq_t last_k = 29; /* start searching from 30 */

/* extend the terms array if needed */
static void ensure_capacity(size_t need)
{
    if (term_capacity >= need)
        return;
    size_t newcap = term_capacity ? term_capacity * 2 : 64;
    while (newcap < need)
        newcap *= 2;
    terms = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    term_capacity = newcap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for offset 1, but return 0 in this edge case */

    /* generate terms until we reach the requested index */
    while (term_count < n)
    {
        last_k += 1;
        if (has_adjacent_03or30(last_k) && !has_adjacent_03or30(last_k - 1))
        {
            /* store the new term */
            ensure_capacity(term_count + 1);
            terms[term_count++] = last_k;
        }
    }
    /* offset 1: a(1) is terms[0] */
    return terms[n - 1];
}
