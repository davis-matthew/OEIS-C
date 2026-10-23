/*
 * A182571  Primes that have four terms in their Zeckendorf representation.
 * https://oeis.org/A182571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes whose Zeckendorf representation uses exactly four terms.
 *            The function returns the n‑th such prime (a(1) = 53)
 * Range:     u8 n <= 11, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm keeps a cache of all primes found that satisfy the
   property.  For each candidate prime p it generates the Fibonacci
   numbers needed to test Zeckendorf representations and counts the
   number of terms in the greedy expansion.  When the count is four,
   p is stored.  The cache is expanded on demand and does not rely on
   increasing n; any order of calls works. */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* How to obtain successive primes – this header provides
   uint64_t next_prime(uint64_t n); that returns the smallest prime
   strictly greater than n. */
#include "../utils/next_prime.h"

static seq_t *seq_cache = NULL; /* primes satisfying the property */
static size_t cache_cap = 0, cache_len = 0;

static seq_t *fib_seq   = NULL; /* Fibonacci numbers F1=1, F2=2 … */
static size_t fib_cap  = 0, fib_len = 0;

/* last prime examined – start before the first prime 2 */
static uint64_t last_prime = 2;

/* Ensure that fib_seq contains all Fibonacci numbers up to at least
   the given limit (inclusive).  The sequence uses F1=1, F2=2. */
static void
ensure_fib(seq_t limit)
{
    /* Initialise with the first two Fibonacci numbers if needed */
    if (fib_len < 2) {
        if (fib_cap < 2) {
            fib_cap = 4;
            fib_seq = (seq_t *)realloc(fib_seq, fib_cap * sizeof(seq_t));
        }
        fib_seq[0] = (seq_t)1;
        fib_seq[1] = (seq_t)2;
        fib_len = 2;
    }

    /* Generate further Fibonacci numbers until the last one reaches the limit. */
    while (fib_len > 0 && fib_seq[fib_len - 1] < limit) {
        /* Use a wider type to avoid overflow when computing the sum */
        uint64_t prev1 = (uint64_t)fib_seq[fib_len - 1];
        uint64_t prev2 = (uint64_t)fib_seq[fib_len - 2];
        uint64_t sum   = prev1 + prev2;

        if (sum > (uint64_t)limit) {
            break;
        }

        if (fib_len == fib_cap) {
            size_t new_cap = fib_cap ? fib_cap * 2 : 4;
            seq_t *tmp = (seq_t *)realloc(fib_seq, new_cap * sizeof(seq_t));
            if (!tmp) abort(); /* unrecoverable allocation failure */
            fib_seq = tmp;
            fib_cap = new_cap;
        }
        fib_seq[fib_len++] = (seq_t)sum;
    }
}

/* Append a value to the cache, growing the array if necessary. */
static void
cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq_cache = tmp;
        cache_cap = new_cap;
    }
    seq_cache[cache_len++] = val;
}

/* Count the number of Fibonacci terms used in the greedy Zeckendorf
   representation of n. */
static size_t
zeckendorf_len(seq_t n)
{
    seq_t rem = n;
    size_t count = 0;
    /* Descend from the largest Fibonacci number down to F1. */
    for (idx_t i = fib_len; i-- > 0; ) {
        if (fib_seq[i] <= rem) {
            rem -= fib_seq[i];
            ++count;
        }
    }
    return count;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    /* Lazily compute terms until we have n of them. */
    while (cache_len < n) {
        /* Get the next prime > last_prime. */
        last_prime = next_prime(last_prime + 1);
        seq_t prime_val = (seq_t)last_prime;

        /* Make sure the Fibonacci table covers this prime. */
        ensure_fib(prime_val);

        /* If the Zeckendorf representation has exactly four terms,
           store the prime. */
        if (zeckendorf_len(prime_val) == 4) {
            cache_append(prime_val);
        }
    }
    return seq_cache[n - 1];
}
