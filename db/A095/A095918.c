/*
 * A095918  Numbers k such that k + (largest digit of k)! is prime.
 * https://oeis.org/A095918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n)  =  smallest k ≥ (n‑1)th term + 1 such that
   k + (largest decimal digit of k)! is prime.
   The function generates terms on demand and caches them. */

/* This implementation uses a simple dynamic array; it works for
   ordinary unsigned integer types and for an arbitrary‑precision
   integer type that is trivially constructible from assignment. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Pre‑computed factorials of digits 0–9 (fits in 64 bit). */
static const uint64_t fact_digit[10] = {
    1ULL, 1ULL, 2ULL,  6ULL,
   24ULL,120ULL,720ULL,5040ULL,
 40320ULL,362880ULL
};

/* Return the largest decimal digit of x (x≥0). */
static inline int largest_digit(idx_t x)
{
    int r = 0;
    while (x > 0) {
        int d = (int)(x % 10ULL);
        if (d > r) r = d;
        x /= 10ULL;
    }
    return r;
}

/* Ensure the dynamic array `*p` has at least `needed` elements. */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **p)
{
    if (needed > *cap) {
        size_t new_cap = (*cap ? *cap * 2 : 16);
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        *p = tmp;
        *cap = new_cap;
    }
}

/* Return the n‑th term of A095918 (1‑based index). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached terms */
    static size_t cache_len = 0;    /* number of cached terms */
    static size_t cache_cap = 0;    /* allocated capacity */
    static idx_t next_k = 1;        /* next integer to test */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        idx_t k = next_k;
        int d = largest_digit(k);
        uint64_t sum = (uint64_t)k + fact_digit[d];
        if (is_prime((idx_t)sum)) {
            ensure_capacity(cache_len + 1, &cache_cap, &cache);
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;
    }
    return cache[n - 1];
}
