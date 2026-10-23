/*
 * A044695  Numbers n such that string 7,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has70
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute force search. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has70.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache    = NULL;
    static idx_t   capacity = 0;
    static idx_t   count    = 0;   /* number of terms stored          */
    static seq_t   next     = 1;   /* next integer to test            */

    /* grow cache if necessary */
    if (count == capacity) {
        size_t new_cap = capacity == 0 ? 16 : capacity * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* generate terms until we reach the nth one */
    while (count < n) {
        seq_t val = next;
        if (has70(val) && !has70(val + 1)) {
            cache[count] = val;
            ++count;
        }
        next = next + 1;
    }

    return cache[n - 1];
}
