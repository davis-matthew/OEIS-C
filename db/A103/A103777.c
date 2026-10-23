/*
 * A103777  Numbers k such that f(k), f(k+1) and f(k+2) are all primes, where
 *          f(k) = 8*k^2 + 4*k + 1.
 * https://oeis.org/A103777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 131, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the first n values of k such that
   f(k)=8*k^2+4*k+1, f(k+1), and f(k+2) are all prime.
   The results are cached to allow random‑order queries. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* primality test for 64‑bit unsigned integers */
#include "../utils/is_prime_uint64.h"

/* ---- f(k) = 8*k^2 + 4*k + 1, computed in 128‑bit to avoid overflow ---- */
static uint64_t f_of(uint64_t k)
{
    __uint128_t v = (__uint128_t)k * k;          /* k² */
    v = v * 8 + (__uint128_t)4 * k + 1;         /* 8*k² + 4*k + 1 */
    return (uint64_t)v;
}

/* ---- resize helper for the cache array ---- */
static void ensure_capacity(size_t *cap, size_t needed, seq_t **arr)
{
    if (*cap >= needed) return;
    size_t new_cap = (*cap > 0) ? *cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp) abort();      /* terminate on out‑of‑memory */
    *arr = tmp;
    *cap = new_cap;
}

/* ---- main sequence generator ---- */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0, sz = 0;
    static uint64_t last_k = 0;      /* last k that was checked */

    if (n == 0) return 0;            /* undefined, avoid UB */

    while (sz < n) {
        uint64_t k = last_k + 1;
        uint64_t f0 = f_of(k);
        uint64_t f1 = f_of(k + 1);
        uint64_t f2 = f_of(k + 2);

        if (is_prime_uint64(f0) && is_prime_uint64(f1) && is_prime_uint64(f2)) {
            ensure_capacity(&cap, sz + 1, &cache);
            cache[sz++] = (seq_t)k;
        }
        last_k = k;
    }

    return cache[n - 1];
}
