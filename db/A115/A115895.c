/*
 * A115895  Numbers k such that k + sigma(k) is a palindrome.
 * https://oeis.org/A115895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 189, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_palindrome, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates OEIS A115895: numbers k such that k + σ(k) is a palindrome.
   The function incrementally tests successive integers k and stores those
   for which k + σ(k) is a palindrome.  It memoises the discovered terms
   so subsequent calls with any n ≥ 1 return the correct value without
   repeating work. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_palindrome.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;   /* array of found terms (k values)   */
    static size_t len   = 0;       /* number of stored terms             */
    static size_t cap   = 0;       /* current capacity of the array      */
    static idx_t last_checked = 0; /* last k that was examined           */

    if (n == 0) return 0; /* offset is 1; undefined for n==0 */

    /* Produce terms until we have at least n of them. */
    while (len < n) {
        last_checked++;                            /* next k to test           */
        seq_t k   = (seq_t)last_checked;           /* cast idx_t to seq_t      */
        seq_t tot = k + sigma_of(last_checked);    /* k + σ(k)                 */

        if (is_palindrome(tot, 10)) {              /* check base‑10 palindrome*/
            /* Grow array if necessary.  Use seq_realloc to handle
               potential move semantics for arbitrary‑precision seq_t. */
            if (len == cap) {
                size_t newcap = cap ? cap * 2 : 4;
                terms = seq_realloc(terms, cap, newcap);
                cap = newcap;
            }
            terms[len++] = k;                      /* store valid term        */
        }
    }

    return terms[n - 1];                           /* 1‑based indexing       */
}
