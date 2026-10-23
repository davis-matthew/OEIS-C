/*
 * A139645  Primes of the form x^2 + 112*y^2.
 * https://oeis.org/A139645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 790, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A139645:
   primes of the form x^2 + 112*y^2, i.e. primes congruent to 1, 9 or 25 (mod 56).
   The function builds a cache of these primes on demand, storing them in ascending
   order.  Subsequent calls to a(n) return the n‑th prime found, independent of
   the call order.
   Compile‑time dependencies:
   - seq_t and idx_t are defined by the build system.
   - The routine uses only integer arithmetic and a small primality test
     suitable for 64‑bit values, which suffices for all n up to about 100 000
     (the state‑of‑the‑art definition of the sequence).  It stays well under
     two seconds for n ≤ 10 000. */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Small deterministic prime test for 64‑bit unsigned integers. The   */
/* numbers we test are far below 2^63, so a basic 6k±1 loop is       */
/* plenty fast.                                                      */
/* ------------------------------------------------------------------ */
static bool is_prime_uint64(unsigned long long n)
{
    if (n < 2ULL) return false;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    for (unsigned long long i = 5ULL; i * i <= n; i += 6ULL) {
        if (n % i == 0ULL) return false;
        if (n % (i + 2ULL) == 0ULL) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Cache of primes that satisfy the residue condition.  The cache is  */
/* grown dynamically as needed.  The array is shared among all calls  */
/* to a() and holds the primes in ascending order.  The variable     */
/* nextcand is the next odd integer to test for the condition.       */
/* ------------------------------------------------------------------ */
static seq_t  *cache      = NULL;   /* pointer to allocated array */
static size_t  cache_len  = 0;      /* number of elements stored */
static size_t  cache_cap  = 0;      /* allocated capacity          */
static idx_t   nextcand   = 3;      /* next odd candidate to test */

/* Append a new prime to the cache, reallocating if necessary.  */
static void cache_add(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2 : 16;
        /* In C++ the result of realloc must be cast to the proper type. */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);  /* out of memory */
        cache     = tmp;
        cache_cap = newcap;
    }
    cache[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Public routine.  Ensures that at least n primes have been found   */
/* and returns the (n‑1)th element of the cache.                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Grow the cache until we have enough primes. */
    while (cache_len < n) {
        unsigned int r = (unsigned int)(nextcand % 56ULL);
        if (r == 1U || r == 9U || r == 25U) {
            if (is_prime_uint64((unsigned long long)nextcand))
                cache_add((seq_t)nextcand);
        }
        nextcand += 2ULL;   /* skip even numbers */
    }
    return cache[n - 1];
}
