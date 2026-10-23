/*
 * A139501  Primes of the form x^2 + 21x*y + y^2 for x and y nonnegative.
 * https://oeis.org/A139501
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: sieve numbers up to a bound and collect primes that can be expressed
 * as x^2 + 21xy + y^2 for non‑negative integers x,y (OEIS A139501) */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"   /* provides grow_seq(seq_t **, size_t *, size_t) */

static uint8_t *rep_bits = NULL;          /* bit array of representable numbers */
static uint8_t *prime_flags = NULL;       /* array of composite flags 0=prime,1=composite */
static seq_t *primes = NULL;              /* list of primes found */
static size_t primes_len = 0;             /* number of primes stored */
static size_t primes_cap = 0;             /* capacity of the primes array */
static size_t cache_limit = 0;            /* current upper bound of the sieve */

/* set a bit in rep_bits to mark a number as representable */
static inline void set_bit(uint8_t *arr, idx_t n)
{
    arr[n >> 3] |= 1u << (n & 7);
}

/* test whether a bit is set in rep_bits */
static inline bool get_bit(const uint8_t *arr, idx_t n)
{
    return (arr[n >> 3] >> (n & 7)) & 1u;
}

/* build or extend the cache until at least req_len primes are available */
static void build_cache(idx_t req_len)
{
    if (primes_len >= (size_t)req_len)
        return;                           /* already have enough */

    /* start with a modest limit */
    if (cache_limit == 0)
        cache_limit = 2000000ULL;          /* 2 million */

    for (;;) {
        free(rep_bits);
        free(prime_flags);
        free(primes);

        rep_bits    = (uint8_t*)calloc((cache_limit >> 3) + 1, 1);
        prime_flags = (uint8_t*)calloc(cache_limit + 1, 1);
        primes      = NULL;
        primes_len  = 0;
        primes_cap  = 0;

        /* generate all numbers of the form x^2 + 21*x*y + y^2 up to cache_limit */
        for (uint64_t x = 0; (uint64_t)x * (uint64_t)x <= cache_limit; ++x) {
            uint64_t xx = x * x;
            for (uint64_t y = 0;; ++y) {
                unsigned __int128 v128 = (unsigned __int128)xx
                                        + (unsigned __int128)21 * x * y
                                        + (unsigned __int128)y * y;
                if (v128 > cache_limit)
                    break;
                set_bit(rep_bits, (idx_t)v128);
            }
        }

        /* sieve of Eratosthenes up to cache_limit */
        for (uint64_t i = 2; i * i <= cache_limit; ++i) {
            if (!prime_flags[i]) {
                for (uint64_t j = i * i; j <= cache_limit; j += i)
                    prime_flags[j] = 1u;
            }
        }
        prime_flags[0] = prime_flags[1] = 1u;   /* 0 and 1 are not prime */

        /* collect primes that are representable */
        for (uint64_t v = 2; v <= cache_limit; ++v) {
            if (!prime_flags[v] && get_bit(rep_bits, v)) {
                if (primes_len == primes_cap)
                    grow_seq(&primes, &primes_cap, primes_len + 1);
                primes[primes_len++] = (seq_t)v;
            }
        }

        if (primes_len >= (size_t)req_len)
            break;                          /* enough primes obtained */

        cache_limit *= 2;                    /* increase bound and retry */
    }
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n = 0 */
        return (seq_t)0;
    build_cache(n);             /* ensure we have at least n primes */
    return primes[(size_t)n - 1];   /* 1‑based indexing */
}
