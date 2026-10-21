/*
 * A085446  Numbers divisible by twice the sum of the products of each of
 *          their digits, excluding even multiples of 10.
 * https://oeis.org/A085446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th positive integer k (0‑based) such that * k % (2
 *            * (sum_of_digits(k))^2) == 0 and * k % 10 != 0
 * Range:     u8 n <= 0, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum_seq_t.h"

/* static cache of the sequence terms */
static seq_t *cache  = NULL;     /* array of terms */
static size_t  cap   = 0;        /* current capacity of cache */
static size_t  sz    = 0;        /* number of terms stored */
static seq_t   start = 1;        /* next candidate to test */

/* ensure cache has at least 'needed' capacity */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cap) return;
    size_t newcap = (cap > 0) ? cap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!tmp) exit(1); /* out of memory */
    cache = tmp;
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 0) return 0;   /* out of bounds, not used */

    /* generate terms up to index n on demand */
    while (sz <= (size_t)n) {
        seq_t x = start;
        start++;  /* increment start for next candidate */

        seq_t s = digit_sum_seq_t(x);           /* sum of digits */
        seq_t divisor = (seq_t)2 * s * s;       /* 2 * (sum)^2 */
        /* check divisibility by divisor and exclude multiples of 10 */
        if (divisor != 0 && (x % divisor) == 0 && (x % (seq_t)10) != 0) {
            ensure_cache_capacity(sz + 1);
            cache[sz] = x;
            sz++;
        }
    }

    return cache[(size_t)n];
}
