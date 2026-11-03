/*
 * A171013  In the sequence of prime numbers, replace all digits '1' with '0'
 *          and vice versa.
 * https://oeis.org/A171013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform( p_n ) where p_n is the nth prime and
 *            `transform` replaces every decimal digit 1 by 0 and every digit
 *            0 by 1 (other digits unchanged)
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 815 (2026-11-01)
 * Helpers:   is_prime_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_seq.h"

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Transform a prime by swapping decimal digits 0↔1. */
static seq_t
transform_prime(seq_t p)
{
    seq_t res   = 0;
    seq_t place = 1;
    while (p > 0)
    {
        uint64_t digit = (uint64_t)(p % 10);   /* extract last decimal digit */
        if (digit == 0)
            digit = 1;
        else if (digit == 1)
            digit = 0;
        res += (seq_t)digit * place;          /* build result in reverse */
        place *= 10;
        p /= 10;
    }
    return res;
}

/* Cached list of primes (as seq_t). */
static seq_t *prime_cache = NULL;      /* array of primes */
static size_t  prime_cap   = 0;        /* allocated capacity          */
static size_t  prime_cnt   = 0;        /* number of primes stored     */
static uint64_t candidate  = 2;        /* next integer to test for primality */

/* Ensure the cache contains at least `n` primes. */
static void
ensure_primes(idx_t n)
{
    while (prime_cnt < n)
    {
        if (is_prime_seq((seq_t)candidate))
        {
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap * 2 : 16;
                seq_t *tmp = seq_realloc(prime_cache, prime_cap, new_cap);
                prime_cache = tmp;
                prime_cap   = new_cap;
            }
            prime_cache[prime_cnt++] = (seq_t)candidate;
        }
        candidate++;                /* advance to next candidate */
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 is guaranteed by the OEIS offset. */
    ensure_primes(n);
    seq_t p = prime_cache[n - 1];
    return transform_prime(p);
}
