/*
 * A181346  Absolute difference between (sum of previous terms) and prime(n)
 *          with a(0) = 1 and a(1) = 2.
 * https://oeis.org/A181346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = | sum_{i=0}^{n-1} a(i) - prime(n) |, a(0)=1, a(1)=2
 * Range:     u8 n <= 54, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static seq_t sum_of_terms = 0;

static void ensure_up_to(idx_t n)
{
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_len <= (size_t)n) {
        seq_t term;
        idx_t idx = (idx_t)cache_len;
        if (idx == 0) {
            term = 1;
        } else if (idx == 1) {
            term = 2;
        } else {
            seq_t prime_val = (seq_t)get_prime(idx); /* prime(idx) with 1‑based indexing */
            if (sum_of_terms >= prime_val)
                term = sum_of_terms - prime_val;
            else
                term = prime_val - sum_of_terms;
        }
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len] = term;
        cache_len++;
        sum_of_terms += term;
    }
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[(size_t)n];
}
