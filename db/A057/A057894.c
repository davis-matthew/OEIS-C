/*
 * A057894  Negabinary numbral "primes".
 * https://oeis.org/A057894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th non‑negative integer k such that the numeric
 *            value obtained by interpreting the binary representation of k as
 *            a base –2 number is ± a conventional prime. The function returns
 *            k
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* Cached sequence values (indices). */
static size_t cache_cap = 0;     /* Allocated capacity of cache. */
static size_t cache_len = 0;     /* Number of elements stored. */
static idx_t next_m = 2;         /* Next candidate index to test. */

/* Ensure that cache contains entries up to (and including) index n. */
static void ensure_term_up_to(idx_t n)
{
    while (cache_len <= n)
    {
        /* Evaluate [next_m] as a base –2 number. */
        long long val   = 0;
        long long power = 1;
        idx_t temp = next_m;
        while (temp)
        {
            if (temp & 1)
                val += power;
            power *= (long long)(-2);
            temp >>= 1;
        }

        long long abs_val = val >= 0 ? val : -val;

        if (abs_val > 1 && is_prime_big((seq_t)abs_val))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();     /* Allocation failed. */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_m;
        }
        ++next_m;
    }
}

/* Return the n-th term of sequence A057894. */
seq_t a(idx_t n)
{
    if (cache_len <= n)
        ensure_term_up_to(n);
    return cache[n];
}
