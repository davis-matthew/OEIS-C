/*
 * A087505  Numbers k such that 5*k+3 is a prime.
 * https://oeis.org/A087505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 7071, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087505: numbers k such that 5*k+3 is prime. */

#include <stdbool.h>
#ifdef __cplusplus
#include <vector>
#endif
#include <stdlib.h>
#include <stdint.h>

/* Simple deterministic primality test suitable for 64‑bit values. */
static bool isPrime(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    idx_t i = 5, w = 2;
    for (; i <= n / i; i += w, w = 6 - w)
        if (n % i == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
#ifndef __cplusplus
    /* C implementation: dynamic array with realloc. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_k = 0;

    while (cache_len < n) {
        idx_t candidate = 5 * next_k + 3;
        if (isPrime(candidate)) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;          /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[n - 1];
#else
    /* C++ implementation: use std::vector. */
    static std::vector<seq_t> cache;
    static idx_t next_k = 0;

    while (cache.size() < n) {
        idx_t candidate = 5 * next_k + 3;
        if (isPrime(candidate))
            cache.push_back((seq_t)next_k);
        ++next_k;
    }
    return cache[n - 1];
#endif
}
