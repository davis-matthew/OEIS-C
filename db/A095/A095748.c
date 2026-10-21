/*
 * A095748  Almost maximally asymmetric primes in binary.
 * https://oeis.org/A095748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A095748: primes whose binary
   representation contains exactly one interior pair of matching
   mirrored bits (besides the obligatory outermost “1‑1” pair). */

/* Use a dynamic cache of found terms; search only odd candidates
   and test whether they meet the interior‑pair criterion. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

/* Return the number of bits required to represent a non‑zero seq_t. */
static inline size_t bitlen_seq_t(seq_t x)
{
    size_t len = 0;
    while (x) {
        ++len;
        x >>= 1;
    }
    return len;
}

/* Check that prime p has exactly one interior matching pair of bits. */
static inline bool has_single_inner_match(seq_t p)
{
    if (p <= 2) return false;           /* skip 2 and below */
    size_t len = bitlen_seq_t(p);
    if (len < 3) return false;          /* at least one interior pair needed */

    /* Only compare interior bits, excluding the first and last. */
    size_t interior = (len - 2) / 2;
    size_t matches  = 0;
    for (size_t i = 1; i <= interior; ++i) {
        seq_t left  = (p >> i)                & 1;
        seq_t right = (p >> (len - 1 - i))    & 1;
        if (left == right) {
            if (++matches > 1)
                return false;               /* more than one interior match */
        }
    }
    return matches == 1;
}

/* Simple dynamic cache of sequence terms. */
static seq_t *cache   = NULL;   /* array of found terms */
static size_t cache_len = 0;    /* number filled so far */
static size_t cache_cap = 0;    /* current capacity */
static uint64_t next_candidate = 3;    /* 3 is the first odd prime */

/* Ensure space for at least one more term. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real program we would handle allocation failure, but we
           assume the build environment provides ample memory. */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Sequence element a(n), 1‑based index. */
seq_t a(idx_t n)
{
    while (cache_len < n) {
        /* Test candidate for primality and the interior‑pair property. */
        if (is_prime_small(next_candidate) &&
            has_single_inner_match((seq_t)next_candidate))
        {
            if (cache_len == cache_cap)
                grow_cache();
            cache[cache_len++] = (seq_t)next_candidate;
        }
        next_candidate += 2;   /* skip even numbers */
    }
    return cache[n - 1];
}
