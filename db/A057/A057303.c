/*
 * A057303  Numbers n such that the number of distinct digits in n is a digit
 *          of n.
 * https://oeis.org/A057303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   satisfies_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: Find the n‑th integer such that the number of distinct
   decimal digits of the integer is a digit that occurs in the integer. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/satisfies_seq_t.h"

/* Static cache of already computed terms.  The cache grows as needed. */
static seq_t *cache = NULL;          /* array of terms */
static idx_t cache_capacity = 0;     /* current allocated size */
static idx_t cache_size = 0;         /* number of terms stored */
static seq_t next_candidate = 1;     /* next integer to test */

/* Return the n-th term (1‑based) of sequence A057303. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Generate terms until we have enough */
    while (cache_size < n) {
        /* Ensure capacity for a new term */
        if (cache_size == cache_capacity) {
            idx_t new_cap = cache_capacity == 0 ? 64 : cache_capacity * 2;
            seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!p) abort();          /* out of memory */
            cache = p;
            cache_capacity = new_cap;
        }

        seq_t candidate = next_candidate;
        if (satisfies_seq_t(candidate)) {
            cache[cache_size++] = candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
