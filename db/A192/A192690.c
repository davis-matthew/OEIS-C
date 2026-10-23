/*
 * A192690  Nonprime numbers with a nonprime number of nonprime divisors.
 * https://oeis.org/A192690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nonprime numbers with a nonprime number of nonprime divisors
   (OEIS A192690).  A term is nonprime and the number of its
   divisors that are not prime is itself nonprime.  The sequence
   is built by scanning the positive integers and memoising the
   qualifying numbers so calls need not be in increasing order. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x); */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static seq_t *cache      = NULL;   /* stored sequence terms   */
static idx_t  cache_cap  = 0;      /* capacity for cache      */
static idx_t  cache_len  = 0;      /* number of terms cached  */
static idx_t  next_cand  = 1;      /* next positive integer to test */

/* Grow the cache to at least 'need' entries */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (size_t)cache_cap << 1 : 16;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = (idx_t)new_cap;
}

/* Test whether m is a sequence member */
static bool qualifies(uint64_t m)
{
    if (m == 1)                    /* 1 is nonprime and qualifies */
        return true;

    if (is_prime((idx_t)m))        /* skip primes */
        return false;

    /* Factor m and compute its divisor count τ(m) */
    uint64_t pr[64], ex[64];
    size_t  cnt = 0;
    factor_u64(m, pr, ex, &cnt);

    seq_t tau = 1;                 /* number of divisors */
    for (size_t i = 0; i < cnt; ++i)
        tau *= (seq_t)(ex[i] + 1);

    seq_t distinct = (seq_t)cnt;   /* number of distinct prime factors */
    seq_t diff = tau - distinct;   /* number of non‑prime divisors */

    /* diff is at least 1.  It must NOT be prime. */
    if (is_prime((idx_t)diff))
        return false;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                   /* undefined for n<1; return 0 */
        return 0;

    /* Ensure the cache holds at least n terms */
    while (cache_len < n) {
        if (qualifies(next_cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_cand;
        }
        ++next_cand;
    }
    return cache[n - 1];
}
