/*
 * A173553  Numbers n such that 5+38*n^2 is not a prime.
 * https://oeis.org/A173553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n such that 5 + 38*n^2 is composite (not prime)
 * Range:     u8 none, u16 n <= 11, u32 n <= 19, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* The prime test for arbitrary‑precision seq_t values. */
#include "../utils/is_prime_big.h"

static seq_t *cache   = NULL;   /* cached terms of the sequence */
static size_t cache_size = 0;   /* number of terms already stored  */
static size_t cache_cap   = 0;   /* current capacity of the cache    */
static idx_t last_checked  = 1;   /* smallest n not yet tested       */

/* Ensure the cache holds at least n terms. */
static void expand_cache_to(idx_t n)
{
    while (cache_size < (size_t)n)
    {
        /* Test the next candidate n = last_checked. */
        idx_t c = last_checked++;
        seq_t c_sq  = (seq_t)c * (seq_t)c;          /* c^2          */
        seq_t val   = c_sq * (seq_t)38;             /* 38*c^2       */
        val += (seq_t)5;                            /* 5+38*c^2     */

        /* If 5+38*c^2 is not prime, store c in the cache. */
        if (!is_prime_big(val))
        {
            /* Grow the cache if needed. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return;   /* allocation failure – abort */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)c;
        }
    }
}

/* Return the n-th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;        /* undefined for n = 0, but just return 0 */
    expand_cache_to(n);
    return cache[n - 1];
}
