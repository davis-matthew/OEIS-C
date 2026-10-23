/*
 * A121167  a(1)=1; a(n) = the reversal of (a(n-1) + spd(a(n-1))), where
 *          spd(n) is the sum of d^d for d the digits of n (with 0^0 = 1).
 * https://oeis.org/A121167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 20, u64 n <= 9393,
 *            u128 n <= 9393, big any n
 * Helpers:   get_digits, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1
   a(n) = reverse_decimal( a(n-1) + spd( a(n-1) ) )
   spd(m) = sum of d^d for each decimal digit d of m
   (with 0^0 defined as 1) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/reverse_decimal.h"

/* Pre‑computed values of d^d (0^0 = 1) for d = 0 … 9 */
static const seq_t pow_table[10] = {
    (seq_t)1,           /* 0^0 */
    (seq_t)1,           /* 1^1 */
    (seq_t)4,           /* 2^2 */
    (seq_t)27,          /* 3^3 */
    (seq_t)256,         /* 4^4 */
    (seq_t)3125,        /* 5^5 */
    (seq_t)46656,       /* 6^6 */
    (seq_t)823543,      /* 7^7 */
    (seq_t)16777216,    /* 8^8 */
    (seq_t)387420489    /* 9^9 */
};

/* Sum of d^d for the decimal digits of x */
static seq_t spd_func(seq_t x)
{
    uint8_t buf[1024];
    size_t len;
    get_digits(x, buf, &len);            /* buf holds the digits of x */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += pow_table[buf[i]];
    return sum;
}

/* Memoised computation of a(n) (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)       /* should never happen for this sequence */
        return 0;

    /* static cache: 1‑based indexing */
    static seq_t *cache = NULL;      /* array of size cache_cap */
    static size_t cache_cap = 0;     /* allocated size */
    static size_t cache_len = 0;     /* last computed index */

    /* ensure capacity */
    if (n + 1 > cache_cap) {
        size_t new_cap = n + 1 > 16 ? n + 1 : 16;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) abort();    /* allocation failure */
        if (cache) {
            for (size_t i = 1; i <= cache_len; ++i)
                new_cache[i] = cache[i];
            free(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute any missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[i] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t spd  = spd_func(prev);
            seq_t sum  = prev + spd;
            cache[i] = reverse_decimal(sum);
        }
    }
    cache_len = n;  /* last computed index */

    return cache[(size_t)n];
}
