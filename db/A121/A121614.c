/*
 * A121614  Numbers that have composite sum of digits and prime sum of squares
 *          of digits.
 * https://oeis.org/A121614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n-th integer whose sum of digits is composite
   and whose sum of squares of digits is prime.  The sequence starts
   27, 45, 54, 72, ...   (OEIS A121614) */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n) {
    /* static state used to memoize the sequence up to the requested index */
    static seq_t *cache = NULL;        /* array holding found terms */
    static size_t cache_len = 0;       /* number of stored terms   */
    static size_t cache_cap = 0;       /* allocated capacity       */
    static seq_t next_candidate = 1;   /* next integer to test     */

    /* If we already have the answer, return it immediately */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Expand the cache until we have at least n terms */
    while (cache_len < n) {
        seq_t x = next_candidate;

        /* Compute sum of digits and sum of squares of digits */
        seq_t sum_digits = 0;
        seq_t sum_sq_digits = 0;
        seq_t t = x;
        while (t > 0) {
            seq_t d = t % 10;
            sum_digits += d;
            sum_sq_digits += d * d;
            t /= 10;
        }

        /* Check the required properties */
        bool is_sum_digits_composite = (sum_digits > 1 && !isPrimeGeneric(sum_digits));
        bool is_sum_sq_digits_prime   = isPrimeGeneric(sum_sq_digits);

        if (is_sum_digits_composite && is_sum_sq_digits_prime) {
            /* Ensure memory for one more term */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed – abort with an indeterminate value.
                       The build system assumes this never happens for the
                       requested range of n. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }

        /* Advance to the next integer to test */
        next_candidate = x + 1;
    }

    return cache[n - 1];
}
