/*
 * A140634  Primes congruent to 41 or 169 mod 210.
 * https://oeis.org/A140634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime that is congruent to 41 or 169 modulo 210
 *            (i.e., primes p such that p % 210 == 41 || p % 210 == 169)
 * Range:     u8 n <= 2, u16 n <= 270, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"

/* The sequence type and index type are provided by the build. */
extern seq_t *seq_alloc_array(size_t n); /* not used, but allowed to be present */

/* Internal static state for memoization */
static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_count = 0;       /* number of values currently cached */
static size_t cache_capacity = 0;    /* allocated capacity of cache */

static uint64_t next_k = 0;          /* current multiplier for 210 */
static int next_r = 0;               /* 0 -> residue 41, 1 -> residue 169 */
static const uint16_t residues[2] = {41, 169};

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (cache_capacity >= needed)
        return;
    size_t new_cap = cache_capacity ? cache_capacity * 2 : 64;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort(); /* out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; behavior for n < 1 is irrelevant */

    /* Generate terms until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        uint64_t candidate = 210ULL * next_k + (uint64_t)residues[next_r];
        /* Skip 1, but all candidates are >= 41, so this check is for completeness */
        if (candidate >= 2 && is_prime_small(candidate))
        {
            /* Store the found prime */
            ensure_capacity(cache_count + 1);
            cache[cache_count++] = (seq_t)candidate;
        }

        /* Advance to the next candidate */
        next_r++;
        if (next_r == 2)
        {
            next_r = 0;
            next_k++;
        }
    }

    return cache[n - 1];
}
