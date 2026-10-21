/*
 * A213519  Numbers that are the sum of cubes of distinct primes.
 * https://oeis.org/A213519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th smallest integer that can be expressed as a sum
 *            of the cubes of distinct primes. The sequence is indexed from 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Static cache for pre‑computed values.  The first 20 primes are enough
   to generate more than 10 000 distinct sum‑of‑cubes values. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool initialized = false;

/* Comparator for qsort on seq_t values. */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (!initialized)
    {
        /* First 20 primes – enough for > 10,000 terms. */
        const uint32_t primes[20] = {
            2U,  3U,  5U,  7U, 11U, 13U, 17U, 19U,
           23U, 29U, 31U, 37U, 41U, 43U, 47U, 53U,
           59U, 61U, 67U, 71U
        };
        const size_t m = 20;

        /* Compute cubes of the primes in seq_t. */
        seq_t *cube = (seq_t *)malloc(m * sizeof(seq_t));
        for (size_t i = 0; i < m; ++i)
        {
            seq_t p = (seq_t)primes[i];
            cube[i] = p * p * p;          /* safe: multiplication in seq_t */
        }

        /* Enumerate all subset sums: 2^m entries. */
        const size_t total = 1UL << m;
        cache = (seq_t *)malloc(total * sizeof(seq_t));
        cache[0] = (seq_t)0;              /* empty subset */

        /* Use the identity sum(mask) = sum(mask_without_lowest_bit) + cube[bit]. */
        for (size_t mask = 1; mask < total; ++mask)
        {
            size_t bit = __builtin_ctzll(mask);            /* lowest set bit */
            size_t prev = mask & (mask - 1);              /* mask without that bit */
            cache[mask] = cache[prev] + cube[bit];
        }

        free(cube);

        /* Sort all sums. */
        qsort(cache, total, sizeof(seq_t), cmp_seq);

        /* Compact into unique sequence. */
        size_t uniq = 0;
        for (size_t i = 0; i < total; ++i)
        {
            if (uniq == 0 || cache[i] != cache[uniq - 1])
            {
                cache[uniq++] = cache[i];
            }
        }
        cache_len = uniq;
        initialized = true;
    }

    /* Sequence is 1‑based.  Behaviour for n < 1 is undefined, but we
       return 0 for safety. */
    if (n <= 0 || (size_t)(n - 1) >= cache_len)
        return (seq_t)0;          /* out of bounds – should not happen in tests */

    return cache[(size_t)(n - 1)];
}
