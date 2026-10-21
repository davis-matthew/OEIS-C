/*
 * A066002  Sum of digits of 6^n.
 * https://oeis.org/A066002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of 6^n
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Compute a(n) = sum of the decimal digits of 6^n.
   The result fits in the type seq_t (for n up to about 10 000
   the sum is < 80 000).  For any build the function is
   memoised; it works in any order of calls. */
seq_t a(idx_t n)
{
    /* ----------------------  static data  ---------------------- */
    static seq_t *cache      = NULL;    /* cached a(k) values */
    static size_t cache_len  = 0;
    static size_t cache_cap  = 0;

    static unsigned char *digits  = NULL;    /* decimal digits of 6^k,
                                              least‑significant digit first */
    static size_t digits_len  = 0;
    static size_t digits_cap  = 0;

    /* ---------------------  initialise on first call  ---------- */
    if (!cache) {
        /* cache for a(0) */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;                  /* a(0) = sum of digits of 1 */
        cache_len = 1;

        /* digits for 6^0 = 1 */
        digits_cap = 16;
        digits = (unsigned char *)malloc(digits_cap * sizeof(unsigned char));
        if (!digits) return (seq_t)0;
        digits[0] = 1;
        digits_len = 1;
    }

    /* --------------------------  answer request  ----------------- */
    if (n < (idx_t)cache_len)
        return cache[n];                     /* already known value */

    /* ----------------------  extend cache to n  ------------------ */
    for (idx_t idx = (idx_t)cache_len; idx <= n; ++idx) {
        /* multiply current number (stored in `digits`) by 6 */
        unsigned int carry = 0;
        for (size_t i = 0; i < digits_len; ++i) {
            unsigned int tmp = (unsigned int)digits[i] * 6u + carry;
            digits[i] = (unsigned char)(tmp % 10u);
            carry = tmp / 10u;
        }
        while (carry > 0) {
            if (digits_len == digits_cap) {
                digits_cap = digits_cap ? digits_cap * 2 : 16;
                unsigned char *tmp = (unsigned char *)
                    realloc(digits, digits_cap * sizeof(unsigned char));
                if (!tmp) return (seq_t)0;   /* allocation failure */
                digits = tmp;
            }
            digits[digits_len++] = (unsigned char)(carry % 10u);
            carry /= 10u;
        }

        /* compute the sum of the digits */
        seq_t sum = 0;
        for (size_t i = 0; i < digits_len; ++i)
            sum += digits[i];

        /* store the newly computed a(idx) */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache,
                                          cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;      /* allocation failure */
            cache = tmp;
        }
        cache[cache_len++] = sum;
    }

    return cache[n];
}
