/*
 * A088785  Primes yielding other primes after each digit is incremented by 2
 *          with carries ignored.
 * https://oeis.org/A088785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1062, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A088785: primes p such that each decimal digit of p increased by
 * 2 (mod 10) gives another prime.  The code searches primes in order,
 * applies the digit mapping, tests primality using the provided isPrime
 * routine, and memoises the results so that a(n) can be called
 * arbitrarily often. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

static const int digit_map[10] = {2,3,4,5,6,7,8,9,0,1};

static seq_t transformed_prime(seq_t n)
{
    seq_t res = 0;
    seq_t mul = 1;
    while (n > 0) {
        int d  = (int)(n % 10);
        int d2 = digit_map[d];
        res += (seq_t)d2 * mul;
        mul *= (seq_t)10;
        n   /= (seq_t)10;
    }
    return res;
}

/* Static cache state – all variables are static so that state is kept
   across calls. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of elements stored so far */
static idx_t cache_cap  = 0;   /* allocated capacity of cache array */
static idx_t next_candidate = 2;  /* next integer to test for primality */
static idx_t last_computed = 0;   /* highest index for which cache[n-1] is valid */

/* Ensure cache has at least 'needed' slots allocated. */
static void ensure_cache_capacity(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Append a qualifying prime to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity(cache_size + 1);
    cache[cache_size++] = val;
}

/* Generate primes up to index n, storing qualifying primes. */
static void compute_up_to(idx_t n)
{
    while (cache_size < n) {
        idx_t p = next_candidate++;
        if (!isPrime(p)) continue;
        seq_t q = transformed_prime((seq_t)p);
        if (isPrime((idx_t)q)) {
            cache_append((seq_t)p);
        }
    }
    last_computed = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value before the offset is undefined */
    if (n > last_computed) {
        compute_up_to(n);
    }
    return cache[n - 1];
}
