/*
 * A039159  Numbers whose base-11 representation has the same number of 0's
 *          and 4's.
 * https://oeis.org/A039159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 
   Computes the n‑th number whose base‑11 representation contains the
   same number of 0's and 4's.  The function enumerates positive
   integers and keeps those that satisfy the condition; once the
   requested index has been reached it returns the result.
*/
seq_t a(idx_t n)
{
    /* Static state for memoisation and quick increment */
    static seq_t *cache   = NULL;    /* Array of found terms (1‑based) */
    static size_t cache_cap = 0;     /* Allocated capacity (number of slots) */
    static idx_t  found_cnt  = 0;    /* How many terms stored so far */
    static seq_t  last_value = 0;    /* Highest integer inspected */

    /* If n is already cached, return directly */
    if (n <= found_cnt)
        return cache[n - 1];

    /* Ensure we have enough capacity to store new terms */
    if (found_cnt >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1; /* Double capacity */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – just return 0 in this unlikely case */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n */
    while (found_cnt < n) {
        last_value++;                 /* Move to the next integer */

        seq_t t = last_value;
        unsigned zeros = 0, fours = 0;

        /* Count 0's and 4's in the base‑11 representation */
        while (t != 0) {
            uint8_t d = (uint8_t)(t % (seq_t)11);
            if (d == 0)   zeros++;
            else if (d == 4) fours++;
            t /= (seq_t)11;
        }

        if (zeros == fours) {
            /* Store the new term */
            cache[found_cnt] = last_value;
            found_cnt++;

            /* Grow the buffer if necessary for the next insert */
            if (found_cnt >= cache_cap) {
                size_t new_cap = cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
        }
    }

    return cache[n - 1];
}
