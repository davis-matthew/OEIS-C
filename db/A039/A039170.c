/*
 * A039170  Numbers whose base-11 representation has the same number of 1's
 *          and 7's.
 * https://oeis.org/A039170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   base11_equal_1_7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A039170: Numbers whose base‑11 representation has equal
 * number of 1’s and 7’s. This function returns the n‑th term (1‑based). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base11_equal_1_7.h"

seq_t a(idx_t n)
{
    /* static cache of found values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t next_start = 0; /* next candidate to test */

    if (n == 0) {
        /* offset is 1; behaviour for n==0 is undefined, return 0 */
        return (seq_t)0;
    }

    /* Extend cache until it contains at least n elements */
    while (len < n) {
        seq_t candidate = next_start;
        if (base11_equal_1_7(candidate)) {
            /* grow cache if necessary */
            if (len >= cap) {
                size_t new_cap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* allocation failure, return 0 */
                    return (seq_t)0;
                }
                cache = tmp;
                cap = new_cap;
            }
            cache[len++] = candidate;
        }
        next_start++;
    }

    return cache[n - 1];
}
