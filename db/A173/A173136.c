/*
 * A173136  Odd nonprimes n such that exactly one of 2n-1 and 2n+1 is prime.
 * https://oeis.org/A173136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   Odd nonprime n such that exactly one of 2n-1 and 2n+1 is prime
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 940 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Cached sequence values and state */
static seq_t *cache = NULL;      /* dynamic array of found terms   */
static size_t cache_size = 0;    /* number of terms stored        */
static size_t cache_cap = 0;     /* capacity of the array         */
static idx_t next_candidate = 1; /* next odd number to test       */

/* Return the n-th term (1-indexed) of A173136. */
seq_t a(idx_t n)
{
    /* Extend the cache until we have at least n terms. */
    while (cache_size < n) {
        /* current candidate is always odd */
        seq_t val = (seq_t)next_candidate;

        /* skip primes */
        if (!isPrimeGeneric(val)) {
            /* compute 2*val */
            seq_t two = mul_small(val, (unsigned int)2);

            /* test primality of 2*val±1 */
            bool pa = isPrimeGeneric(two - (seq_t)1);
            bool pb = isPrimeGeneric(two + (seq_t)1);

            /* accept if exactly one of them is prime */
            if (pa ^ pb) {
                /* grow the cache if needed */
                if (cache_size == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) {
                        /* Out of memory – terminate the program. */
                        exit(EXIT_FAILURE);
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_size++] = val;
            }
        }

        /* move to the next odd number */
        next_candidate += 2;
    }

    /* Return the requested element (offset 1). */
    return cache[n - 1];
}
