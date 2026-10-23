/*
 * A160109  Numbers k with the property that k^2 uses only nonsquare decimal
 *          digits (2,3,5,6,7,8).
 * https://oeis.org/A160109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth positive integer k such that k^2 uses only the
 *            nonsquare decimal digits 2,3,5,6,7,8. The function generates the
 *            sequence by testing successive integers
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   k_square_allowed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/k_square_allowed.h"

/* Cache of found terms.  Stored as seq_t which may be an arbitrary‑precision
   integer or a small unsigned integer type. */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;   /* number of stored terms          */
static size_t seq_cap   = 0;   /* capacity of seq_cache           */
static uint64_t last_k  = 0;   /* last k examined                 */

/* Append a new found term to the cache, growing it if necessary. */
static void append_term(uint64_t k)
{
    if (seq_len == seq_cap) {
        size_t new_cap = (seq_cap == 0 ? 4 : seq_cap * 2);
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = (seq_t)k;
}

/* External function required by the build.  Returns the nth term. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n<1; return 0 for safety */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        ++last_k;
        if (k_square_allowed(last_k))
            append_term(last_k);
    }
    return seq_cache[n - 1];
}
