/*
 * A024894  Numbers k such that 5*k + 1 is prime.
 * https://oeis.org/A024894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 2251, u32 n <= 668, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A024894: numbers k such that 5*k + 1 is prime.
   The nth term a(n) is the n‑th such k (a(1)=2). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Ensure a dynamic array has room for at least needed elements. */
static void
ensure_capacity(size_t needed, seq_t **arr, size_t *cap)
{
    if (needed > *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (tmp) {
            *arr = tmp;
            *cap = newcap;
        }
    }
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored k‑values */
    static size_t cache_len = 0;    /* number of entries in cache */
    static size_t cache_cap = 0;    /* current allocated capacity */
    static idx_t next_k = 1;        /* next k to test */

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        unsigned long long cand = 5ULL * next_k + 1ULL;   /* 5*k+1 as 64‑bit integer */
        seq_t candidate = (seq_t)cand;                   /* cast to seq_t for primality test */

        if (is_prime_big(candidate)) {   /* test primality */
            ensure_capacity(cache_len + 1, &cache, &cache_cap);
            if (cache_len < n) {           /* cache may have grown by ensure_capacity */
                cache[cache_len++] = (seq_t)next_k;
            }
        }
        ++next_k;
    }
    return cache[n - 1];
}
