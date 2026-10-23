/*
 * A161349  Positive numbers with property that the digital sum plus the
 *          product of the digits is a square.
 * https://oeis.org/A161349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, digit_sum, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force enumeration of integers for which  
 * the sum of digits plus the product of digits is a perfect square. */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

#define CACHE_CAP 200000  /* big enough for n up to ~10000 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined for the offset but safe */

    static seq_t cache[CACHE_CAP];       /* cached terms */
    static size_t len = 0;               /* number of cached terms */
    static idx_t  next = 1;              /* candidate to test next  */

    while (len < n) {
        seq_t x = (seq_t)next;           /* current candidate               */
        seq_t s = digit_sum(x);          /* digit sum                       */
        seq_t p = digit_product(x);      /* product of digits               */
        seq_t sum = s + p;               /* sum of sum and product          */
        seq_t t   = floor_sqrt(sum);     /* floor of sqrt of sum            */
        if (t * t == sum) {              /* property holds?                 */
            cache[len++] = x;            /* store the term                   */
        }
        ++next;                          /* advance to next integer          */
    }

    return cache[n - 1];
}
