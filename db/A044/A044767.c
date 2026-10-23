/*
 * A044767  Numbers k such that string 5,4 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains54_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search: a(n) is the nth integer k such that the decimal
 * representation of k contains the substring "54" while the
 * representation of k+1 does not. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains54_idx.h"

/* Dynamic cache of already found sequence terms */
static seq_t  *seq_cache   = NULL;   /* array of terms                */
static size_t seq_cap     = 0;       /* allocated capacity            */
static size_t seq_len     = 0;       /* number of terms stored        */
static idx_t  last_k      = 1;       /* next integer to test         */
static idx_t  terms_found = 0;       /* terms computed so far         */

static void grow_cache(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) abort();   /* allocation failure – terminate program */
    seq_cache = new_arr;
    seq_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not defined for n=0 */

    /* Generate terms until we have at least n of them */
    while (terms_found < n) {
        /* Scan integers starting from last_k to find the next term */
        while (true) {
            if (contains54_idx(last_k) && !contains54_idx(last_k + 1)) {
                /* store the term */
                if (seq_len == seq_cap) grow_cache();
                seq_cache[seq_len++] = (seq_t)last_k;
                terms_found++;

                /* prepare for next search */
                last_k++;                 /* next integer to test */
                break;
            }
            last_k++;
        }
    }

    return seq_cache[n - 1];
}
