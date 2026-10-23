/*
 * A160861  The digits of each number can be divided into two groups which
 *          have the same product.
 * https://oeis.org/A160861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   has_equal_product_partition
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160861: numbers whose decimal digits can be partitioned
 * into two non‑empty groups having the same product.
 * Search‑based sequential generation. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_equal_product_partition.h"

/* Main sequence routine.  Results are memoised so that repeated calls
 * are fast even if n is requested in arbitrary order. */
seq_t a(idx_t n) {
    static idx_t computed = 0;          /* how many terms we have stored */
    static seq_t next_to_check = 1;     /* next integer to test */
    static size_t cache_cap = 0;        /* allocated size of cache */
    static seq_t *cache = NULL;         /* array of found terms */

    if (n == 0) {
        return 0; /* undefined for n<1, but return 0 as safety */
    }

    /* Grow cache if needed */
    if (n > cache_cap) {
        size_t new_cap = (n > 1) ? n : 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – abort (caller should handle). */
            exit(1);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute until we have at least n terms */
    while (computed < n) {
        while (!has_equal_product_partition(next_to_check)) {
            ++next_to_check;
        }
        /* Store found term */
        cache[computed++] = next_to_check;
        ++next_to_check;
    }

    return cache[n - 1];
}
