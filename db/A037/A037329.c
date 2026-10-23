/*
 * A037329  Numbers whose base-6 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = n‑th positive integer whose base‑6 and base‑9 digit
   sums are equal. */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* cached sequence values              */
static idx_t  cache_size = 0;      /* number of values stored             */
static idx_t  cache_cap  = 0;      /* allocated capacity of cache          */
static seq_t  next_val   = 1;      /* next natural number to test         */

seq_t a(idx_t n)
{
    /* Ensure we have at least n entries in the cache.   */
    while (cache_size < n) {
        seq_t sum6 = 0, sum9 = 0, x;

        /* Compute digit sum in base 6. */
        x = next_val;
        while (x > 0) {
            sum6 += x % 6;
            x    /= 6;
        }

        /* Compute digit sum in base 9. */
        x = next_val;
        while (x > 0) {
            sum9 += x % 9;
            x    /= 9;
        }

        /* If sums match, store the number. */
        if (sum6 == sum9) {
            if (cache_size == cache_cap) {
                idx_t new_cap = cache_cap ? (cache_cap << 1) : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_size++] = next_val;
        }

        /* Move to the next integer. */
        next_val += 1;
    }
    return cache[(size_t)(n - 1)];
}
