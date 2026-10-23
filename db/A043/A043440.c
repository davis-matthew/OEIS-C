/*
 * A043440  Numbers having four 4's in base 8.
 * https://oeis.org/A043440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   count_octal_fours
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer whose base‑8 representation
   contains exactly four digits 4.  Computed by scanning all integers
   in increasing order.
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/count_octal_fours.h"

/* Lazy generation storage */
static seq_t *cache = NULL;       /* array of found numbers */
static size_t  cap   = 0;         /* allocated capacity */
static size_t  size  = 0;         /* number of elements stored */
static seq_t   next  = 0;         /* next candidate to test */

/* Generate all terms up to and including index n (1‑based). */
static void build_up_to(idx_t n)
{
    while (size < n) {
        ++next;                    /* move to next integer */
        if (count_octal_fours(next) == 4) {
            if (size >= cap) {
                size_t new_cap = cap ? cap * 2 : 64;
                seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (!new_cache)
                    return;        /* allocation failure – give up */
                if (cache) {
                    memcpy(new_cache, cache, size * sizeof(seq_t));
                    free(cache);
                }
                cache = new_cache;
                cap   = new_cap;
            }
            cache[size++] = next;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n < 1, but safe fallback */

    build_up_to(n);
    return cache[n - 1];
}
