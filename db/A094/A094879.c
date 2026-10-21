/*
 * A094879  Where 1's occur in A094830 (nonprimes that reach a prime in one
 *          step under iteration of "x -> x + sum of squares of digits of x").
 * https://oeis.org/A094879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n‑th positive integer x (x≥1) such that * x is NOT
 *            prime and x + Σ(digit²) is prime. * The sequence lists such x in
 *            increasing order
 * Range:     u8 n <= 14, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array, get_digits, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>                /* size_t */

#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */
#include "../utils/get_digits.h"   /* void get_digits(seq_t x, uint8_t *buf, size_t *len); */
#include "../utils/alloc_seq_array.h"  /* seq_t *alloc_seq_array(size_t n); */
#include "../utils/free_seq_array.h"   /* void free_seq_array(seq_t *p); */

/* compute the sum of squares of the decimal digits of x */
static seq_t digit_square_sum(seq_t x)
{
    uint8_t buf[64];
    size_t len;
    get_digits(x, buf, &len);
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned int d = buf[i];
        sum += (seq_t)(d * d);
    }
    return sum;
}

/* grow an array of seq_t, retaining old values */
static seq_t *grow_cache(seq_t *cache, size_t old_cap, size_t new_cap)
{
    seq_t *new_cache = alloc_seq_array(new_cap);
    for (size_t i = 0; i < old_cap; ++i) {
        new_cache[i] = cache[i];
    }
    free_seq_array(cache);
    return new_cache;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, safe return */

    /* static state: cache of terms, current test value, etc. */
    static seq_t *cache = NULL;
    static size_t cnt  = 0;                 /* number of stored terms   */
    static size_t cap  = 0;                 /* allocated capacity      */
    static seq_t cur  = 1;                 /* next integer to test    */

    /* generate terms until we have at least n of them */
    while (cnt < n) {
        /* ensure enough capacity */
        if (cap < cnt + 1) {
            size_t new_cap = cap ? cap * 2 : 16;
            if (new_cap < cnt + 1) new_cap = cnt + 1;
            cache = grow_cache(cache, cap, new_cap);
            cap = new_cap;
        }

        /* test whether cur satisfies the condition */
        if (!is_prime_big(cur)) {
            seq_t ns = cur + digit_square_sum(cur);
            if (is_prime_big(ns)) {
                cache[cnt++] = cur;           /* store the term */
            }
        }
        ++cur;                                /* next integer to test */
    }

    return cache[n - 1];                     /* zero‑based index in cache */
}
