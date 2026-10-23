/*
 * A178668  Maximal prime divisor of the average of the twin prime pairs,
 *          different from 2 and 3. In case of maximal prime divisor is 2 or
 *          3, then a(n)=1.
 * https://oeis.org/A178668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest_prime_divisor(p_n + 1) when this > 3, otherwise
 *            a(n) = 1, where p_n is the lower member of the nth twin prime
 *            pair. The sequence is computed by incrementally generating twin
 *            primes and storing the results
 * Range:     u8 n <= 53, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the helper that returns the greatest prime factor of a 64‑bit integer. */
#include "../utils/gpf.h"

#include <stdlib.h>
#include <stdint.h>

/* Integer square root using only integer operations. */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t bit = (uint64_t)1 << 62;   /* largest power of 4 <= 2^64-1 */
    while (bit > n) bit >>= 2;
    uint64_t res = 0;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Dynamic array of primes discovered so far. */
static uint64_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_sz = 0;

/* Append a prime to the list. */
static void push_prime(uint64_t p)
{
    if (prime_sz == prime_cap) {
        size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
        uint64_t *new_lst = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
        if (!new_lst) exit(1);
        prime_list = new_lst;
        prime_cap = new_cap;
    }
    prime_list[prime_sz++] = p;
}

/* Test primality using trial division by stored primes. */
static int is_prime(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 1;
    if ((n & 1) == 0) return 0;
    uint64_t limit = isqrt_u64(n);
    for (size_t i = 0; i < prime_sz; ++i) {
        uint64_t p = prime_list[i];
        if (p > limit) break;
        if (n % p == 0) return 0;
    }
    return 1;
}

/* Cache for computed sequence values. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_sz = 0;   /* number of values stored */

/* State for twin‑prime generation. */
static uint64_t last_candidate = 3;   /* next odd number to test */
static uint64_t last_prime = 0;       /* last prime found */
static idx_t  twin_found = 0;         /* number of twin pairs processed */

/* Ensure that the cache has entries up to at least 'n' (1‑based). */
static void ensure_twin_count(idx_t n)
{
    while ((idx_t)twin_found < n) {
        uint64_t cand = last_candidate;
        if (is_prime(cand)) {
            push_prime(cand);
            if (last_prime != 0 && cand == last_prime + 2) {
                /* Found a twin pair (last_prime, cand).  Compute a value. */
                uint64_t avg = last_prime + 1;  /* average of the pair */
                uint64_t g = gpf(avg);
                seq_t val = (seq_t)(g > 3 ? g : 1);
                if (cache_sz == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!new_cache) exit(1);
                    cache = new_cache;
                    cache_cap = new_cap;
                }
                cache[cache_sz++] = val;
                twin_found++;
            }
            last_prime = cand;
        }
        last_candidate += 2;   /* skip even numbers */
    }
}

/* OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; n=0 is unused */
    ensure_twin_count(n);
    return cache[n - 1];
}
