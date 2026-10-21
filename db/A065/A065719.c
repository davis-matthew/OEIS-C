/*
 * A065719  Number of 8's in decimal expansion of 2^n.
 * https://oeis.org/A065719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of 8's in the decimal expansion of 2^n.
   a(0) = 0, a(n) = count of digit '8' in decimal(2^n). */

/* This implementation keeps a static cache of previously computed values
   and the decimal digits of the current power of two.  For each new
   requested n it multiplies the stored decimal number by 2 (digit‑wise),
   counts the 8’s, stores the result and expands the cache if required.  */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Static cache of results a(0), a(1), ... */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of cached entries (next free index) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    /* Static buffer holding the decimal digits of 2^current_n.
       Least‑significant digit first. */
    static uint8_t digits[4000];
    static idx_t digits_len = 1;  /* number of used digits */

    /* Initialise on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* 2^0 = 1 has no 8’s */
        cache[0] = 0;
        cache_len = 1;
        digits[0] = 1;          /* 2^0 = 1 */
    }

    /* Compute values up to n if necessary. */
    while (cache_len <= n) {
        /* Multiply the current decimal number by 2. */
        int carry = 0;
        for (idx_t i = 0; i < digits_len; ++i) {
            int prod = (int)digits[i] * 2 + carry; /* prod <= 18 + 1 = 19 */
            if (prod >= 10) {
                digits[i] = (uint8_t)(prod - 10);
                carry = 1;
            } else {
                digits[i] = (uint8_t)prod;
                carry = 0;
            }
        }
        if (carry) {
            digits[digits_len++] = (uint8_t)carry;  /* carry is 1 */
        }

        /* Count the number of 8’s in this decimal representation. */
        seq_t cnt = 0;
        for (idx_t i = 0; i < digits_len; ++i) {
            if (digits[i] == 8) {
                ++cnt;
            }
        }

        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap ? cache_cap * 2 : 4;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = cnt;
    }

    return cache[n];
}
