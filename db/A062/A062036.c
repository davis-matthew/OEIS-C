/*
 * A062036  Positive numbers whose product of digits is four times their sum.
 * https://oeis.org/A062036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 237, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A062036: the n‑th positive integer whose product of digits
   equals four times its sum of digits.  The function searches numbers in
   increasing order, caches the results, and can be called in any order.
   The digit product and sum are computed directly to avoid reliance on
   possibly buggy utility functions. */
#include <stdlib.h>
#include <stddef.h>

/* Digit product – returns 0 if the number contains a 0 digit. */
static seq_t digit_prod(seq_t x)
{
    if (x == 0)               /* the only number with digit 0 in all places */
        return 0;
    seq_t prod = 1;
    while (x > 0) {
        int d = (int)(x % 10);
        if (d == 0)
            return 0;         /* product vanishes */
        prod *= d;
        x /= 10;
    }
    return prod;
}

/* Digit sum. */
static seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* External linkage – the required signature. */
seq_t a(idx_t n)
{
    if (n == 0)               /* no term 0 – return 0 as a harmless sentinel */
        return 0;

    /* Cache of already found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static seq_t max_checked = 0;  /* largest integer tested so far */

    /* Return cached value if present. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Scan candidates until we have n terms. */
    seq_t candidate = max_checked + 1;
    while (cache_len < (size_t)n) {
        seq_t prod = digit_prod(candidate);
        seq_t sum  = digit_sum(candidate);
        if (prod == 4 * sum) {                 /* property satisfied */
            if (cache_len == cache_cap) {       /* grow cache if needed */
                cache_cap = cache_cap ? cache_cap * 2 : 4;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
        ++candidate;
    }

    /* Update the maximum tested number. */
    max_checked = candidate - 1;
    return cache[n - 1];
}
