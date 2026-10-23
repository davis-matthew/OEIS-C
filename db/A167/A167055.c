/*
 * A167055  Numbers k such that 12*k + 5 is prime.
 * https://oeis.org/A167055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the k-th non‑negative integer where 12*k+5 is prime (with
 *            a(1)=0)
 * Range:     u8 n <= 113, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Simple deterministic 64‑bit primality test. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Return the n‑th term (n ≥ 1) of sequence A167055. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cache_len = 0;  /* number of cached terms */
    static size_t cache_cap = 0;  /* allocated capacity */
    static uint64_t next_k = 0;   /* next k value to test */

    if (n == 0) return 0; /* undefined input – return 0 for safety */

    /* Grow the cache if necessary. */
    if (cache_len < n) {
        if (cache == NULL) {
            cache_cap = 64;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        } else if (cache_cap < cache_len + (n - cache_len)) {
            /* Double until large enough. */
            while (cache_cap < n) cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Generate terms until we have enough. */
        while (cache_len < n) {
            uint64_t val = 12 * next_k + 5;
            if (is_prime_uint64(val)) {
                cache[cache_len++] = (seq_t)next_k;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
