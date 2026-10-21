/*
 * A226227  Numbers m such that all lengths of runs of 0's in the binary
 *          representation of m are prime numbers.
 * https://oeis.org/A226227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose binary representation has only *
 *            prime-length runs of 0's. This is computed by brute‑force
 *            enumeration
 * Range:     u8 n <= 70, u16 n <= 4409, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Test if an unsigned integer is prime (works for small values). */
static inline bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    if (x == 2 || x == 3) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (unsigned int i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

/* Return true iff every run of 0's in the binary representation of m
 * has a prime length. */
static inline bool is_good(seq_t m)
{
    unsigned int zero_len = 0;
    while (m != 0)
    {
        if ((m & 1) == 0)
        {
            zero_len++;
        }
        else
        {
            if (zero_len > 0)
            {
                if (!is_prime_small(zero_len))
                    return false;
                zero_len = 0;
            }
        }
        m >>= 1;
    }
    /* All runs have been examined. */
    return true;
}

/* Return the n-th term of A226227 (offset 1). */
seq_t a(idx_t n)
{
    /* Cache of previously generated terms. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 0;

    if (n == 0)
        return 0;

    /* Allocate initial cache. */
    if (cache == NULL)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache_len = 0;
    }

    /* Generate terms until we have at least n terms. */
    while (cache_len < n)
    {
        seq_t m = last_checked + 1;
        for (;; ++m)
        {
            if (is_good(m))
            {
                if ((size_t)cache_len == cache_cap)
                {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = m;
                if (cache_len == n)
                {
                    last_checked = m;
                    return m;
                }
            }
        }
    }

    return cache[n - 1];
}
