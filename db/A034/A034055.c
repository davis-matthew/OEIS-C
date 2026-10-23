/*
 * A034055  Numbers with multiplicative digital root value 8.
 * https://oeis.org/A034055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 4384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A034055: Numbers with multiplicative digital root value 8.
 * For a positive integer x, let M(x) be the multiplicative digital
 * root obtained by repeatedly multiplying the decimal digits of x
 * until a single digit is reached.  This function returns the n-th
 * integer x (counting from 1) with M(x)=8.
 */
#include <stdlib.h>
#include "../utils/digit_product.h"

/* compute multiplicative digital root of a number */
static seq_t mdr(seq_t x)
{
    while (x >= 10) {
        x = digit_product(x);
    }
    return x;
}

/* cache of already-found terms */
static seq_t *cache     = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* below offset, undefined */
    /* generate terms up to n */
    while (cache_len < (size_t)n) {
        seq_t candidate;
        if (cache_len == 0) {
            candidate = (seq_t)1;
        } else {
            candidate = cache[cache_len - 1] + (seq_t)1;
        }
        while (1) {
            if (mdr(candidate) == (seq_t)8) {
                /* store it */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = candidate;
                break;
            }
            candidate++;
        }
    }
    return cache[n - 1];
}
