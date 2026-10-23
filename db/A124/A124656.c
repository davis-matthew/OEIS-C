/*
 * A124656  Abundant odious numbers. (Abundant numbers that have an odd number
 *          of ones in their binary representation.)
 * https://oeis.org/A124656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th abundant odious number * (an abundant number
 *            that has an odd number of 1-bits in its binary representation)
 * Range:     u8 n <= 21, u16 n <= 7334, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- caching of generated terms ------------------------------------------- */
static seq_t *cache       = NULL;   /* array of stored values       */
static size_t cache_len   = 0;       /* number of terms actually cached   */
static size_t cache_cap   = 0;       /* capacity of cache array  */
static idx_t next_candidate = 1;     /* next integer to examine for inclusion */

/* Helper to ensure at least 'required' terms have been generated.  */
static void ensure_terms(idx_t required)
{
    if (required <= (idx_t)cache_len)
        return;

    /* Grow cache array if needed. */
    if (required > (idx_t)cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)required)
            new_cap = (size_t)required;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan integers starting from 'next_candidate' until we have enough terms. */
    while ((size_t)cache_len < required) {
        idx_t x = next_candidate;
        /* --- test for abundance ------------------------------------------------ */
        unsigned long long n = x;
        unsigned long long sigma = 1;
        unsigned long long temp = n;

        for (unsigned long long p = 2; p * p <= temp; ++p) {
            if (temp % p == 0) {
                int exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                /* sum_{k=0}^{exp} p^k */
                unsigned long long term = 1;
                unsigned long long pe = 1;
                for (int k = 0; k < exp; ++k) {
                    pe *= p;
                    term += pe;
                }
                sigma *= term;
            }
        }
        if (temp > 1) {           /* remaining prime factor */
            unsigned long long p = temp;
            unsigned long long term = 1 + p;   /* (p^0 + p^1) since exp = 1 for leftover */
            sigma *= term;
        }
        bool abundant = sigma > (2 * n);

        /* --- test for odd popcount ------------------------------------------ */
        bool odd_pop = (__builtin_popcountll(n) & 1) != 0;

        if (abundant && odd_pop)
            cache[cache_len++] = (seq_t)x;
        ++next_candidate;
    }
}

/* OEIS entry access function. */
seq_t a(idx_t n)
{
    if (n == 0)     /* index 0 is undefined for this sequence (offset 1) */
        return 0;
    ensure_terms(n);
    return cache[n - 1];
}
