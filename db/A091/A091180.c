/*
 * A091180  Primes of the form 3*p - 2 such that p is a prime.
 * https://oeis.org/A091180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A091180: primes of the form 3*p - 2 with p prime.
    This implementation searches for primes p and tests whether 3*p-2 is prime.
    The sequence is 1‑indexed.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* state for searching primes p */
static uint64_t next_prime_candidate = 3;  /* next odd number to test for being prime */

/* small prime test using trial division; works for values that fit in uint64_t */
static bool is_prime_u64(uint64_t n)
{
    if (n <= 1) return false;
    if (n <= 3) return true;
    if ((n & 1ULL) == 0ULL) return false;
    if ((n % 3ULL) == 0ULL) return false;

    for (uint64_t i = 5ULL; i <= n / i; i += 6ULL) {
        if ((n % i) == 0ULL || (n % (i + 2ULL)) == 0ULL)
            return false;
    }
    return true;
}

/* grow the cache array if necessary */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* append a new term to the cache */
static void append_cache(seq_t val)
{
    ensure_cache_capacity(cache_len + 1);
    cache[cache_len++] = val;
}

/* generate terms until the cache contains at least n values */
static void extend_cache(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* find the next prime p */
        while (!is_prime_u64(next_prime_candidate))
            next_prime_candidate += 2ULL;

        /* compute candidate = 3*p - 2 as a 128‑bit value then cast to seq_t */
        unsigned __int128 cand128 = (unsigned __int128)3 * next_prime_candidate - 2;
        seq_t cand = (seq_t)cand128;

        /* test if 3*p-2 is prime (only if it fits in uint64_t) */
        if (cand128 <= UINT64_MAX && is_prime_u64((uint64_t)cand128))
            append_cache(cand);

        /* move to the next odd number for prime search */
        next_prime_candidate += 2ULL;
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)  /* sequence is 1‑indexed; undefined for n=0 */
        return (seq_t)0;

    extend_cache(n);
    return cache[(size_t)n - 1];
}
