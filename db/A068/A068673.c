/*
 * A068673  Numbers which yield primes when a 1 is prefixed or appended.
 * https://oeis.org/A068673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A068673: a(n) = the n-th positive integer n such that 
 *    10^digits(n)+n is prime and n*10+1 is prime
 * This implementation iteratively searches numbers and memoizes the
 * results so subsequent calls are fast.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    /* static state for memoization */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_len = 0;     /* number of terms stored */
    static size_t cache_cap = 0;     /* allocated capacity */
    static idx_t last_checked = 0;   /* last integer checked */

    /* variables for power of 10 management */
    static seq_t pow10 = 0;         /* 10^digits(current) */
    static idx_t next_limit = 0;    /* next integer where digit count increases */
    static int digit_len = 0;       /* current number of digits */

    /* one-time initialization */
    if (cache_len == 0) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        pow10 = (seq_t)10;       /* 10^1 for numbers 1..9 */
        next_limit = 10;         /* next multiplication threshold */
        digit_len = 1;
        last_checked = 0;
    }

    /* return immediately if we already have the term */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* allocate more space when needed */
    if (cache_len >= cache_cap) {
        cache_cap *= 2;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* search for new terms until we have at least n of them */
    for (;;) {
        idx_t val = ++last_checked;     /* candidate integer */

        /* update digit length and corresponding power of 10 */
        if (val == next_limit) {
            digit_len++;
            pow10 *= (seq_t)10;
            next_limit *= 10;
        }

        seq_t seq_val = (seq_t)val;
        seq_t prefix = seq_val + pow10;         /* 10^digits + n */
        seq_t suffix = seq_val * (seq_t)10 + 1; /* n*10 + 1 */

        if (is_prime_big(prefix) && is_prime_big(suffix)) {
            /* store the found term */
            cache[cache_len++] = seq_val;
            if (cache_len == n) {   /* we have enough terms */
                return seq_val;
            }
        }
    }
}
