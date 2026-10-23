/*
 * A023280  Primes that remain prime through 3 iterations of function f(x) =
 *          3x + 10.
 * https://oeis.org/A023280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 19, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A023280: primes p such that
   3p+10, 9p+40, and 27p+130 are also primes. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/seq_realloc.h"   /* realloc helper for seq_t */

static seq_t *cache = NULL;      /* cached terms of the sequence       */
static size_t cache_size = 0;    /* number of terms stored             */
static size_t cache_cap = 0;     /* allocated capacity of 'cache'      */
static uint64_t last_prime = 1;  /* last prime inspected (for next prime) */

/* Ensure that the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 16;  /* double size, start at 16 */
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_arr = seq_realloc(cache, cache_cap, new_cap);
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour below offset is undefined */
        return 0;

    while (cache_size < n) {
        /* Get the next prime after the one we considered last time. */
        last_prime = next_prime(last_prime + 1);
        seq_t p = (seq_t)last_prime;

        /* Compute 3p+10, 9p+40, 27p+130 using seq_t arithmetic. */
        seq_t f1 = p * (seq_t)3 + (seq_t)10;          /* 3p + 10          */
        seq_t f2 = f1 * (seq_t)3 + (seq_t)10;         /* 9p + 40          */
        seq_t f3 = f2 * (seq_t)3 + (seq_t)10;         /* 27p + 130        */

        /* Test all three numbers for primality. */
        if (is_prime_big(f1) && is_prime_big(f2) && is_prime_big(f3)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = p;
        }
    }
    return cache[n - 1];
}
