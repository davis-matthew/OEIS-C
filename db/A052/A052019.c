/*
 * A052019  Sum of digits of prime p is substring of p.
 * https://oeis.org/A052019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that the decimal sum of digits of p
 *            appears as a contiguous substring of p itself
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   digit_len, digit_sum, is_prime, seq_to_dec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/digit_len.h"
#include "../utils/is_prime.h"
#include "../utils/seq_to_dec.h"

/* Main function requested by the framework. */
seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap   = 0;
    static idx_t next_candidate = 2;   /* next integer to test for primality */

    /* continue generating until we have at least n terms */
    while (cache_size < n) {
        /* find next prime */
        while (!is_prime(next_candidate))
            ++next_candidate;

        seq_t p   = (seq_t)next_candidate;
        seq_t sum = digit_sum(p);

        /* convert p and sum to strings */
        size_t len_p  = (size_t)digit_len(p);
        size_t len_s  = (size_t)digit_len(sum);
        char *buf_p   = (char *)malloc(len_p + 1);
        char *buf_s   = (char *)malloc(len_s + 1);
        if (!buf_p || !buf_s) { /* safety check: abort on allocation failure */
            free(buf_p);
            free(buf_s);
            return (seq_t)0;
        }
        seq_to_dec(p,   buf_p);
        seq_to_dec(sum, buf_s);

        if (strstr(buf_p, buf_s) != NULL) {
            /* prime satisfies the property – store it */
            if (cache_size == cache_cap) {
                cache_cap = cache_cap * 2 + 10;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = p;
        }

        free(buf_p);
        free(buf_s);
        ++next_candidate;
    }

    return cache[n - 1];
}
