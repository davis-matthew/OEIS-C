/*
 * A144548  Squares and cubes of nonprimes (without repetition).
 * https://oeis.org/A144548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th sorted distinct value of x² or y³ where x and y
 *            are non‑prime integers (including 0 and 1)
 * Range:     u8 n <= 10, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Uses a simple trial‑division primality test suitable for the small
    indices encountered while generating the sequence.      */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Internal primality test for idx_t (uint64_t). 0 and 1 are not prime. */
static bool is_prime_idx(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d * d <= n; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    /* Quick return for no meaningful indices */
    if (n == 0) return (seq_t)0;

    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current candidates for square and cube */
    static idx_t sq_i = 0;
    static idx_t cu_i = 0;

    /* Initialise cache once */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Ensure current indices are non‑prime */
        while (is_prime_idx(sq_i)) sq_i++;
        while (is_prime_idx(cu_i)) cu_i++;

        /* Compute next square and cube values */
        seq_t sq_val = (seq_t)sq_i * (seq_t)sq_i;
        seq_t cu_val = (seq_t)cu_i * (seq_t)cu_i * (seq_t)cu_i;

        /* Merge in ascending order, skipping duplicates */
        if (sq_val < cu_val) {
            if (cache_len == 0 || sq_val != cache[cache_len - 1]) {
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = sq_val;
            }
            sq_i++;
            while (is_prime_idx(sq_i)) sq_i++;
        } else if (cu_val < sq_val) {
            if (cache_len == 0 || cu_val != cache[cache_len - 1]) {
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = cu_val;
            }
            cu_i++;
            while (is_prime_idx(cu_i)) cu_i++;
        } else { /* sq_val == cu_val */
            if (cache_len == 0 || sq_val != cache[cache_len - 1]) {
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = sq_val;
            }
            sq_i++; while (is_prime_idx(sq_i)) sq_i++;
            cu_i++; while (is_prime_idx(cu_i)) cu_i++;
        }
    }

    return cache[n - 1];
}
