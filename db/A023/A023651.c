/*
 * A023651  Numbers k such that (product of digits of k) * (sum of digits of
 *          k) = 2k.
 * https://oeis.org/A023651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = numbers k such that (product of digits of k) * (sum of
 *            digits of k) = 2k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_len  = 0;   /* number of elements stored */
static size_t cache_cap  = 0;   /* current capacity of cache array */
static idx_t next_k      = 0;   /* next candidate integer to test */

/* Ensure the cache array has space for at least one more element. */
static void give_capacity(void) {
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    } else if (cache_len == cache_cap) {
        size_t new_cap = cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp != NULL) {
            cache = tmp;
            cache_cap = new_cap;
        }
    }
}

/* Compute the sequence term at position n (n >= 1). */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined, but keep safe */

    /* Ensure the cache holds at least n elements. */
    while ((idx_t)cache_len < n) {
        give_capacity();

        seq_t cand = (seq_t)next_k;
        seq_t prod, sum;

        if (cand == (seq_t)0) {
            prod = (seq_t)0;
            sum = (seq_t)0;
        } else {
            prod = (seq_t)1;
            sum  = (seq_t)0;
            seq_t tmp = cand;
            while (tmp > (seq_t)0) {
                seq_t digit = tmp % ((seq_t)10);
                prod *= digit;
                sum  += digit;
                tmp  /= ((seq_t)10);
            }
        }

        if (prod * sum == (seq_t)2 * cand) {
            cache[cache_len++] = cand;
        }
        next_k++;
    }

    return cache[n - 1];
}
