/*
 * A117404  Triangular numbers for which the sum of the digits is a square.
 * https://oeis.org/A117404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 7821, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, floor_sqrt, grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force: generate triangular numbers T_k = k*(k+1)/2 and keep those
   whose decimal digit sum is a perfect square.  The sequence a(n) is the
   n‑th such triangular number (n is 1‑based). */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/floor_sqrt.h"
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* Dynamically maintain a cache of the found terms. */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;

    /* Current state of the triangular number generator. */
    static idx_t  next_k = 0;      /* k such that next_T = T_k */
    static seq_t next_T = 0;       /* current triangular number T_k */

    /* Return cached value if already computed. */
    if (n <= length) return cache[n - 1];

    /* Generate triangular numbers until we have at least n terms. */
    while (length < n) {
        /* Check whether the digit sum of next_T is a perfect square. */
        seq_t sum  = digit_sum(next_T);
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            /* Ensure sufficient capacity and store the term. */
            if (length == capacity)
                grow_seq(&cache, &capacity, length + 1);
            cache[length++] = next_T;
        }
        /* Advance to the next triangular number: T_{k+1} = T_k + (k+1). */
        next_k++;
        next_T += next_k;
    }

    return cache[n - 1];
}
