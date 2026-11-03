/*
 * A061522  a(0) = 0; a(n) is obtained by incrementing each digit of a(n-1) by
 *          7.
 * https://oeis.org/A061522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = increment_digits(a(n-1)), a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Increment each decimal digit of x by 7, writing any value ≥10 as
   two decimal digits (e.g., 9→16).  For x == 0 the result is 7. */
static seq_t increment_digits(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)7;

    /* Temporarily store the decimal digits of x (least–significant
       first).  The buffer is grown as needed. */
    size_t dcap = 8;
    unsigned char *digits = (unsigned char *)malloc(dcap);
    if (!digits)
        return (seq_t)0;               /* allocation failure (unlikely) */

    size_t dlen = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        unsigned long long d = (unsigned long long)(t % (seq_t)10);  /* 0‑9 */
        t = t / (seq_t)10;
        if (dlen == dcap) {
            dcap <<= 1;
            unsigned char *p = (unsigned char *)realloc(digits, dcap);
            if (!p) {
                free(digits);
                return (seq_t)0;
            }
            digits = p;
        }
        digits[dlen++] = (unsigned char)d;
    }

    /* Build the new number from most–significant digit to least. */
    seq_t res = (seq_t)0;
    while (dlen--) {
        unsigned int val = (unsigned int)digits[dlen] + 7;  /* 7‑16 */
        if (val >= 10)
            res = res * (seq_t)100 + (seq_t)val;          /* two digits */
        else
            res = res * (seq_t)10 + (seq_t)val;           /* one digit */
    }

    free(digits);
    return res;
}

/* Cache used to handle out‑of‑order calls. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* Initialise the cache with a(0). */
    if (cache_len == 0) {
        cache_cap = 1;
        cache = mem_new(seq_t, cache_cap);
        if (!cache)
            return (seq_t)0;
        cache[0] = (seq_t)0;
        cache_len = 1;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 1;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp)
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = increment_digits(cache[cache_len - 1]);
        cache_len++;
    }

    return cache[(size_t)n];
}
