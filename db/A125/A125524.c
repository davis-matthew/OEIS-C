/*
 * A125524  Republican primes: primes such that the right half of the prime is
 *          prime and the left half is not.
 * https://oeis.org/A125524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_primes, digit_len, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A125524 – Republican primes.
   a(n) is the n‑th prime such that its right half is prime and its left
   half is composite, with the central digit ignored for odd–length
   primes.  The sequence starts at a(2)=13 (offset 2).  This routine
   generates the primes on demand and memoises the results so that any
   order of calls works.                                                       */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/digit_len.h"
#include "../utils/pow10.h"
#include "../utils/is_prime_big.h"
#include "../utils/build_primes.h"

/* --------------------------------------------------------------------- */
/*  Static state ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */
static uint64_t *prime_list   = NULL;   /* primes up to prime_limit          */
static size_t  prime_cnt     = 0;        /* number of primes                   */
static idx_t   prime_limit   = 0;        /* upper bound of current sieve       */
static size_t  next_prime_idx = 0;       /* next prime index to process        */

static seq_t  *cache_ptr     = NULL;    /* cached republican primes          */
static size_t  cache_cap     = 0;        /* capacity of cache_ptr              */
static size_t  cache_cnt     = 0;        /* number of elements stored          */

/* --------------------------------------------------------------------- */
/*  Prime sieve helpers --------------------------------------------------- */
/* --------------------------------------------------------------------- */
static void ensure_primes(idx_t limit)
{
    if (prime_limit >= limit && prime_list != NULL)
        return;

    if (prime_list != NULL)
        free(prime_list);

    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit  = limit;
    next_prime_idx = 0;
}

/* --------------------------------------------------------------------- */
/*  Cache helpers --------------------------------------------------------- */
/* --------------------------------------------------------------------- */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *tmp = (seq_t *)realloc(cache_ptr, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache_ptr = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/*  Helper to split a prime into left/right halves ---------------------- */
/* --------------------------------------------------------------------- */
static void split_prime(uint64_t p, uint64_t *left, uint64_t *right)
{
    int  len = digit_len(p);
    int  k   = len / 2;
    uint64_t powk = (uint64_t)pow10(k);          /* 10^k  (fits in 64 bits) */

    if (len % 2 == 0) {                         /* even digits   */
        *left  = p / powk;
        *right = p % powk;
    } else {                                    /* odd digits – drop the centre */
        uint64_t powk1 = powk * 10;              /* 10^(k+1)     */
        *left  = p / powk1;
        *right = p % powk;
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function ------------------------------------------------ */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2)                         /* undefined below the offset */
        return 0;

    size_t target = (size_t)(n - 1);    /* number of terms we need */

    /* lazily initialise cache */
    if (!cache_ptr)
        ensure_cache_cap(8);

    /* return already cached value */
    if (target <= cache_cnt && cache_ptr[target-1] != 0)
        return cache_ptr[target-1];

    /* ensure we can generate primes on demand */
    if (prime_limit == 0)
        ensure_primes(200000);            /* start with 200k */

    /* grow cache to hold at least the required number of terms */
    ensure_cache_cap(target + 1);

    /* generate terms until we have enough */
    while (cache_cnt < target)
    {
        /* if we ran out of primes in the current sieve, enlarge it */
        if (next_prime_idx >= prime_cnt) {
            idx_t new_limit = prime_limit * 2 + 1000;
            ensure_primes(new_limit);
            continue;
        }

        uint64_t p = prime_list[next_prime_idx++];
        if (p < 10)          /* single‑digit primes cannot satisfy the rule */
            continue;

        uint64_t left, right;
        split_prime(p, &left, &right);

        bool right_is_prime = is_prime_big((seq_t)right);
        bool left_is_prime  = is_prime_big((seq_t)left);

        if (right_is_prime && !left_is_prime) {
            cache_ptr[cache_cnt++] = (seq_t)p;
            if (cache_cnt == target)
                return (seq_t)p;
        }
    }

    /* should never reach here */
    return cache_ptr[target-1];
}
