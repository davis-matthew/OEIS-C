/*
 * A141090  Integral quotients of products of first k consecutive composites
 *          divided by their sums: products (dividends).
 * https://oeis.org/A141090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A141090: product of the first k consecutive composites
 * such that the product divided by their sum is an integer.
 * The function returns the n‑th such product (a(1)=4). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    /* static state to keep progress between calls */
    static seq_t *cache    = NULL;   /* cached sequence values     */
    static size_t  cap     = 0;      /* cache capacity              */
    static size_t  sz      = 0;      /* number of terms stored      */

    static seq_t product = 1;        /* running product             */
    static idx_t  sum     = 0;       /* running sum                 */
    static idx_t  next    = 4;       /* next integer to test        */

    /* allocate or grow the cache as needed */
    if (cap == 0) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    } else if (sz == cap) {
        cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    }

    /* generate terms until we have at least n values */
    while (sz < n) {
        /* find the next composite number */
        while (is_prime(next)) {
            ++next;
        }

        /* update running product and sum */
        product = product * (seq_t)next;
        sum     += next;
        ++next;

        /* test divisibility: product / sum is integer? */
        if (product % (seq_t)sum == (seq_t)0) {
            /* store the product into the cache */
            if (sz == cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = product;
        }
    }

    /* return the requested term */
    return cache[n - 1];
}
