/*
 * A224854  Numbers n such that 90*n + 11 and 90*n + 13 are twin prime.
 * https://oeis.org/A224854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   append_candidate, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers n such that 90*n+11 and 90*n+13 are twin primes. */
/* Uses a lazy cache: successive calls extend the list only as far as needed. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* OEIS offset 1: a(1) is the first valid n. */
    if (n == 0) return 0;          /* guard for accidental call */

    /* Lazy cache of qualifying n's. */
    static seq_t *cache     = NULL;   /* dynamic array of seq_t */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* capacity of current array */
    static idx_t  next_test = 0;     /* next n to check */

    /* Extend cache until it contains at least n terms. */
    while (cache_len < n)
    {
        /* Compute 90*n+11 and 90*n+13 (twin primes). */
        seq_t p1 = (seq_t)next_test * (seq_t)90 + (seq_t)11;
        seq_t p2 = p1 + (seq_t)2;

        if (is_prime_big(p1) && is_prime_big(p2))
        {
            /* Store the qualifying n. */
            append_candidate(&cache, &cache_len, &cache_cap, next_test);
        }

        ++next_test;
    }

    return cache[n - 1];
}
