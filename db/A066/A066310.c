/*
 * A066310  Numbers k such that k < (product of digits of k) * (sum of digits
 *          of k).
 * https://oeis.org/A066310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, fini
 * Formula:   not stated
 * Range:     u8 n <= 141, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A066310: Numbers k such that k < (digit_product(k)) * (digit_sum(k)).
   a(n) is the n‑th such integer (a(1)=2).  The implementation uses a
   simple linear search with caching; it works for any seq_t type that
   supports the usual integer operators. */

#include <stdlib.h>
#include <stdbool.h>

/* Compute the sum of the decimal digits of x. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % 10;
        x   /= 10;
    }
    return sum;
}

/* Compute the product of the decimal digits of x.  If any digit is zero,
   the product is zero. */
static seq_t digit_product(seq_t x)
{
    seq_t prod = 1;
    while (x != 0) {
        seq_t digit = x % 10;
        if (digit == 0)    /* a zero digit makes the whole product zero */
            return 0;
        prod *= digit;
        x   /= 10;
    }
    return prod;
}

seq_t a(idx_t n)
{
    /* Cache of previously found terms. */
    static seq_t *cache = NULL;      /* array holding the terms */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t len   = 0;         /* number of terms stored */
    static seq_t last_checked = 0;   /* largest integer examined so far */

    if (n == 0)           /* out of bounds, offset 1 */
        return 0;

    /* Grow the cache until it contains at least n terms. */
    seq_t k = last_checked + 1;
    while (len < n) {
        seq_t prod = digit_product(k);
        if (prod != 0) {
            seq_t sum = digit_sum(k);
            /* Test k < prod * sum without overflowing. */
            seq_t m = k / sum;          /* floor division, sum>0 */
            if (prod > m) {
                if (len == cap) {
                    size_t newcap = cap == 0 ? 8 : cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                    if (!tmp)
                        return 0;        /* allocation failure */
                    cache = tmp;
                    cap   = newcap;
                }
                cache[len++] = k;
            }
        }
        last_checked = k;
        ++k;
    }
    return cache[n - 1];
}
