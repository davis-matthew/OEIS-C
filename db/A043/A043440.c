/*
 * A043440  Numbers having four 4's in base 8.
 * https://oeis.org/A043440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the nth positive integer whose base‑8 representation
   contains exactly four digits 4.  Computed by scanning all integers
   in increasing order.
*/

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

/* Helper: count the number of digit 4's in the base‑8 representation of x. */
static unsigned count_octal_fours(seq_t x)
{
    unsigned c = 0;
    while (x) {
        if ((x & (seq_t)7) == (seq_t)4) {
            ++c;
            if (c > 4)
                break;
        }
        x >>= 3;            /* divide by 8 */
    }
    return c;
}

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
                seq_t *new_cache = mem_new(seq_t, new_cap);
                if (!new_cache)
                    return;        /* allocation failure – give up */
                if (cache) {
                    mem_copy(new_cache, cache, size * sizeof(seq_t));
                    mem_free(cache);
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
