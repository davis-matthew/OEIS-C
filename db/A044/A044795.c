/*
 * A044795  Numbers k such that string 8,2 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has82
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the decimal string "82"
 * occurs in k but not in k+1 (OEIS A044795). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has82.h"

/* Memoization structures */
static seq_t *terms = NULL;      /* array of found terms */
static size_t terms_len = 0;     /* number of terms stored */
static size_t terms_cap = 0;     /* allocated capacity */
static seq_t scan_next = (seq_t)82;   /* next number to test */

/* Ensure capacity for at least 'needed' terms. */
static void grow_terms(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_terms;
#ifdef __cplusplus
    new_terms = reinterpret_cast<seq_t*>(realloc(terms, new_cap * sizeof(seq_t)));
#else
    new_terms = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
#endif
    if (!new_terms) abort(); /* memory exhaustion */
    terms = new_terms;
    terms_cap = new_cap;
}

/* Return the nth element of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n<1; return 0 as sentinel */
    while (terms_len < n)
    {
        if (has82(scan_next) && !has82(scan_next + (seq_t)1))
        {
            grow_terms(terms_len + 1);
            terms[terms_len++] = scan_next;
        }
        ++scan_next;
    }
    return terms[n - 1];
}
