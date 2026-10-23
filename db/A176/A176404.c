/*
 * A176404  Semiprimes == -+1 (mod 8).
 * https://oeis.org/A176404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 6092, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A176404
   Sequence: Semiprimes ≡ ±1 (mod 8)
   a(n) = the nth number that is a semiprime and congruent to 1 or 7 modulo 8
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for generated sequence and auxiliary data */
static seq_t *cache = NULL;          /* array of generated sequence values   */
static size_t cache_size = 0;        /* number of values stored in cache     */
static size_t cache_cap   = 0;       /* capacity of cache array              */

static uint64_t *primes = NULL;      /* list of primes discovered so far     */
static size_t primes_size = 0;       /* number of primes stored             */
static size_t primes_cap   = 0;       /* capacity of prime list               */

static uint64_t next_checked = 2;    /* next integer to test in the loop     */

/* Grow a dynamic array of seq_t values when needed */
static void grow_cache(void)
{
    if (cache_cap == 0)
    {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    else if (cache_size == cache_cap)
    {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* Grow the prime list array when needed */
static void grow_primes(void)
{
    if (primes_cap == 0)
    {
        primes_cap = 64;
        primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    }
    else if (primes_size == primes_cap)
    {
        primes_cap <<= 1;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; return the n-th term */
    while (cache_size < (size_t)n)
    {
        uint64_t i = next_checked++;
        /* Test primality of i and store if prime */
        bool is_prime = true;
        for (size_t j = 0; j < primes_size; ++j)
        {
            uint64_t p = primes[j];
            if (p * p > i)
                break;
            if (i % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            grow_primes();
            primes[primes_size++] = i;
        }

        /* Check if i is a semiprime and ≡ 1 or 7 (mod 8) */
        uint8_t mod8 = (uint8_t)(i & 7);
        if (mod8 == 1 || mod8 == 7)
        {
            uint64_t temp = i;
            int factors = 0;
            for (size_t j = 0; j < primes_size && primes[j] * primes[j] <= temp; ++j)
            {
                uint64_t p = primes[j];
                while (temp % p == 0 && factors <= 2)
                {
                    temp /= p;
                    ++factors;
                }
                if (factors > 2)
                    break;
            }
            if (factors <= 2 && temp > 1)
                ++factors;  /* remaining prime factor */

            if (factors == 2)
            {
                grow_cache();
                cache[cache_size++] = (seq_t)i;
                continue;
            }
        }
        /* otherwise i is not part of the sequence; continue */
    }

    return cache[(size_t)n - 1];
}
