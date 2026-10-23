/*
 * A038346  Sum of first n primes of form 4k+1.
 * https://oeis.org/A038346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (prime_k of the form 4k+1)
 * Range:     u8 n <= 7, u16 n <= 105, u32 n <= 9976, u64 n <= 9976,
 *            u128 n <= 9976, big any n
 * Helpers:   iSqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/iSqrt_u64.h"

/* --------------------------------------------------------------------- */
/* Utility: integer square root for uint64_t                              */

/* --------------------------------------------------------------------- */
/* Prime list and sum cache                                               */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* primes found so far        */
static size_t prime_cnt   = 0;        /* number of stored primes    */
static size_t prime_cap   = 0;        /* capacity of prime_list     */

static seq_t *sum_cache = NULL;      /* a(1) ... a(current_n)     */
static size_t cache_n   = 0;          /* number of entries stored   */
static size_t cache_cap = 0;          /* capacity of sum_cache      */

/* --------------------------------------------------------------------- */
/* Ensure prime_list capacity                                            */
/* --------------------------------------------------------------------- */
static void ensure_prime_cap(size_t need_cap)
{
    if (need_cap <= prime_cap)
        return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    if (new_cap < need_cap)
        new_cap = need_cap;
    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_list)
        abort();                      /* allocation failure */
    prime_list = new_list;
    prime_cap  = new_cap;
}

/* --------------------------------------------------------------------- */
/* Ensure sum_cache capacity                                             */
/* --------------------------------------------------------------------- */
static void ensure_cache_cap(size_t need_cap)
{
    if (need_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need_cap)
        new_cap = need_cap;
    seq_t *new_cache = (seq_t *)realloc(sum_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                      /* allocation failure */
    sum_cache = new_cache;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Test primality using stored primes                                   */
/* --------------------------------------------------------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    uint64_t r = iSqrt_u64(n);
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > r)
            break;
        if (n % p == 0)
            return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Add a new prime to prime_list if needed                               */
/* --------------------------------------------------------------------- */
static void add_prime(uint64_t candidate)
{
    if (is_prime_uint64(candidate)) {
        ensure_prime_cap(prime_cnt + 1);
        prime_list[prime_cnt++] = candidate;
    }
}

/* --------------------------------------------------------------------- */
/* The sequence function                                                  */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) corresponds to the first 4k+1 prime (5)                 */
    if (n == 0)
        return 0;                      /* undefined, harmless */

    /* Initialize prime list with 2 if empty                         */
    if (prime_cnt == 0) {
        ensure_prime_cap(1);
        prime_list[0] = 2;
        prime_cnt = 1;
    }

    /* Generate primes and sums until we have at least n sums       */
    uint64_t candidate = prime_list[prime_cnt - 1] + 1;
    while (cache_n < (size_t)n) {
        /* Find next prime */
        while (!is_prime_uint64(candidate))
            ++candidate;

        /* Store the new prime */
        add_prime(candidate);

        /* If it is of the form 4k+1, add to sum cache */
        if ((candidate & 3ULL) == 1ULL) {
            seq_t added = (seq_t)candidate;
            seq_t new_sum = (cache_n == 0) ? added : sum_cache[cache_n - 1] + added;
            ensure_cache_cap(cache_n + 1);
            sum_cache[cache_n] = new_sum;
            ++cache_n;
        }
        ++candidate;
    }
    return sum_cache[n - 1];
}
