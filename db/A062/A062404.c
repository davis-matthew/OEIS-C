/*
 * A062404  Numbers k such that 2*k*prime(k) + 1 is prime.
 * https://oeis.org/A062404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 22, u32 n <= 24, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n‑th integer k such that 2*k*prime(k)+1 is prime. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"    /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"/* bool is_prime_big(seq_t n); */

extern seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t next_k = 1;            /* next k to test (1‑based) */

    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < n) {
        /* compute expression 2*k*prime(k)+1 in 128‑bit integer */
        uint32_t pk = get_prime(next_k);
        unsigned __int128 prod = (unsigned __int128)next_k * (unsigned __int128)pk;
        prod = prod * 2 + 1;
        seq_t expr = (seq_t)prod;

        if (is_prime_big(expr)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
            }
            cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return cache[n - 1];
}
