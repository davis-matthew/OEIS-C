/*
 * A180478  Numbers m such that r*m/k is an integer. m=(x_1 x_2 ... x_r) where
 *          x_i are digits of m, k = x_1 * x_2 * ... * x_r.
 * https://oeis.org/A180478
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 504, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_len, digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th integer m such that r*m/k is an integer,
   where r = number of decimal digits of m and
   k = product of the decimal digits of m.  The sequence is
   generated in ascending order. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/digit_len.h"

static seq_t *cache = NULL;         /* memoised sequence values          */
static size_t cache_cap = 0;        /* capacity of cache array           */
static size_t cache_len = 0;        /* number of terms currently stored  */
static seq_t next_candidate = 1;    /* smallest integer not yet tested   */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, return 0 as a graceful default */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        seq_t m = next_candidate++;
        seq_t prod = digit_product(m);          /* k */
        if (prod != 0) {                       /* skip numbers containing a zero digit */
            seq_t r = (seq_t)digit_len(m);      /* digit count r */
            seq_t rem = m % prod;              /* m mod k */
            if (((r * rem) % prod) == 0) {     /* r*m is divisible by k */
                /* Ensure enough space in cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* memory exhaustion */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = m;
            }
        }
    }
    return cache[n - 1];
}
