/*
 * A095085  Fib000 primes, i.e., primes p whose Zeckendorf-expansion
 *          A014417(p) ends with three zeros.
 * https://oeis.org/A095085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of primes whose Zeckendorf representation
 * (with Fibonacci numbers 1, 2, 3, 5, …) uses none of the
 * first three Fibonacci numbers.  Equivalently, the binary Zeckendorf
 * expansion ends with three zeros.  The function performs a
 * simple incremental search of primes and checks each candidate
 * with a greedy Zeckendorf decomposition.  Results are cached
 * so that calls may be made in any order and the same prime set
 * is reused. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/*--------------------------------------------------------------------
 * Fibonacci cache (1‑based indexing: fib[1] = 1, fib[2] = 2).
 *--------------------------------------------------------------------*/
static seq_t *fib = NULL;
static size_t fib_len = 0;       /* number of generated Fibonacci numbers */
static size_t fib_cap = 0;       /* storage capacity */

/* Grow the Fibonacci table until it contains a Fibonacci number
   that is > limit.  The table is kept strictly ascending. */
static void grow_fib_upto(seq_t limit)
{
    /* initialise if required */
    if (!fib) {
        fib_cap = 16;
        fib = (seq_t *)malloc((fib_cap + 1) * sizeof(seq_t));
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)2;
        fib_len = 2;
    }

    /* generate until fib[fib_len] > limit  (strictly greater) */
    while (fib_len < 2 || fib[fib_len] <= limit) {
        if (fib_len + 1 >= fib_cap) {
            fib_cap <<= 1;
            fib = (seq_t *)realloc(fib, (fib_cap + 1) * sizeof(seq_t));
        }
        fib[fib_len + 1] = fib[fib_len] + fib[fib_len - 1];
        fib_len++;
    }
}

/*--------------------------------------------------------------------
 * Test whether the Zeckendorf representation of p does not use any of
 * the first three Fibonacci numbers.
 *--------------------------------------------------------------------*/
static bool z_has_trailing_zeros(seq_t p)
{
    grow_fib_upto(p);

    seq_t rem = p;
    while (rem > 0) {
        /* binary search for largest Fibonacci <= rem */
        size_t lo = 1, hi = fib_len;
        while (lo < hi) {
            size_t mid = (lo + hi + 1) >> 1;
            if (fib[mid] <= rem)
                lo = mid;
            else
                hi = mid - 1;
        }

        if (lo <= 3)          /* one of the first three used */
            return false;

        rem -= fib[lo];
    }
    return true;
}

/*--------------------------------------------------------------------
 * Main entry point for the sequence.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored qualifying primes */
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* candidate primes to test */
    static uint64_t candidate = 2;

    while (cache_len < (size_t)n) {
        if (is_prime((idx_t)candidate)) {
            seq_t p = (seq_t)candidate;
            if (z_has_trailing_zeros(p)) {
                /* enlarge cache if necessary */
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = p;
            }
        }
        candidate++;
    }

    return cache[n - 1];
}
