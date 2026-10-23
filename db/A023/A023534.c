/*
 * A023534  Numbers k such that the largest power of 2 dividing k equals
 *          2^omega(k).
 * https://oeis.org/A023534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that the * exponent of 2
 *            dividing k equals the number of distinct prime * divisors of k.
 *            This is OEIS A023534
 * Range:     u8 n <= 25, u16 n <= 4403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_distinct_primes.h"

/* Static cache of the sequence values */
static seq_t      *seq_cache = NULL;
static size_t      seq_len   = 0;      /* number of terms stored */
static size_t      seq_cap   = 0;      /* current allocation capacity */
static uint64_t    next_k    = 1;      /* smallest integer not yet tested */

/* Grow the cache array to accommodate at least 'need' elements */
static void
grow_seq_cache(size_t need)
{
    if (seq_cap >= need)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Generate sequence terms until we have at least n terms stored */
static void
ensure_n_terms(idx_t n)
{
    /* n fits into size_t for the range we care about */
    size_t need = (size_t)n;
    while (seq_len < need)
    {
        uint64_t k = next_k++;

        /* Compute v_2(k): number of trailing zeros in binary representation */
        unsigned int v2 = 0;
        uint64_t t = k;
        while ((t & 1ULL) == 0)
        {
            ++v2;
            t >>= 1;
        }

        /* Compute ω(k): number of distinct prime factors */
        int omega = count_distinct_primes(k);

        /* Store k if the property holds */
        if ((unsigned int)omega == v2)
        {
            grow_seq_cache(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)k;
        }
    }
}

/* Public sequence function */
seq_t
a(idx_t n)
{
    if (n == 0)
    {
        /* Sequence is defined from offset 1; return 0 for safety */
        return (seq_t)0;
    }
    ensure_n_terms(n);
    return seq_cache[(size_t)(n - 1)];
}
