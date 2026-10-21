/*
 * A066003  Sum of digits of 7^n.
 * https://oeis.org/A066003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(7^n)
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#define BASE 1000000000U    /* 10^9 – base for intermediate big‑integer storage */

seq_t a(idx_t n)
{
    /* ---------- cache of already computed digit sums ---------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated array capacity */
    static size_t cached_upto = 0;    /* largest n for which cache has a value */

    /* ---------- representation of 7^current_n ---------- */
    static uint32_t *digits = NULL;   /* little‑endian words base BASE */
    static size_t digits_cap = 0;
    static size_t digits_len = 0;

    /* ---------- initialise everything on first call ---------- */
    if (cache == NULL) {
        /* cache for digit sums: grow as needed */
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));

        cache[0] = (seq_t)1;   /* a(0) = digit_sum(1) = 1 */
        cached_upto = 0;        /* we have computed up to n = 0 */

        /* digits for 7^0 = 1 */
        digits_cap = 64;
        digits = (uint32_t *)realloc(NULL, digits_cap * sizeof(uint32_t));
        digits[0] = 1;
        digits_len = 1;
    }

    /* ---------- extend cache up to requested n ---------- */
    if (n > cached_upto) {
        for (size_t k = cached_upto + 1; k <= n; ++k) {
            /* --- multiply current number by 7 --- */
            uint64_t carry = 0;
            for (size_t i = 0; i < digits_len; ++i) {
                uint64_t prod = (uint64_t)digits[i] * 7 + carry;
                digits[i] = (uint32_t)(prod % BASE);
                carry    = prod / BASE;
            }
            if (carry > 0) {
                if (digits_len == digits_cap) {
                    size_t new_cap = digits_cap << 1;
                    digits = (uint32_t *)realloc(digits, new_cap * sizeof(uint32_t));
                    digits_cap = new_cap;
                }
                digits[digits_len++] = (uint32_t)carry;
            }

            /* --- compute digit sum of the new number --- */
            seq_t sum = 0;
            for (size_t i = 0; i < digits_len; ++i) {
                uint32_t w = digits[i];
                /* each word contains exactly 9 decimal digits */
                for (int j = 0; j < 9; ++j) {
                    sum += w % 10;
                    w   /= 10;
                }
            }

            /* --- store the result in the cache --- */
            if (k == cache_cap) {
                size_t new_cap = cache_cap << 1;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[k] = sum;
        }
        cached_upto = n;
    }

    return cache[n];
}
