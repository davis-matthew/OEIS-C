/*
 * A217277  Numbers n such that n^2-9 is divisible by consecutive primes
 *          beginning with 2.
 * https://oeis.org/A217277
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest positive integer n such that n^2 – 9 is
 *            divisible by the consecutive primes 2,3,5,… and none of the
 *            primes that follow the longest consecutive block
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 30, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_init = false;

/* initialise a pool of primes up to a reasonable bound.  The bound
   is chosen large enough to cover the prime needed for the first
   several thousands of terms. */
static void init_primes(void)
{
    if (!primes_init)
    {
        idx_t limit = 2000000ULL;      /* about 148k primes */
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_init = true;
    }
}

/* dynamic array for the generated sequence */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

static void ensure_terms_cap(size_t needed)
{
    if (needed > terms_cap)
    {
        size_t new_cap = terms_cap ? terms_cap * 2UL : 16UL;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!ptr) exit(1);                 /* out of memory */
        terms = ptr;
        terms_cap = new_cap;
    }
}

/* test whether the candidate x satisfies the definition of the
   sequence:  x^2 – 9 is divisible by the consecutive primes
   2,3,5,… up to some point and by none of the following primes. */
static bool candidate_valid(seq_t x)
{
    /* compute x^2 – 9 safely */
    unsigned __int128 tmp = (unsigned __int128)x * (unsigned __int128)x - (unsigned __int128)9;
    seq_t val = (seq_t)tmp;

    /* find the first prime that does NOT divide x^2 – 9 */
    uint64_t fail_prime = 0;
    for (size_t i = 1; i < primes_cnt; ++i)   /* skip prime 2 */
    {
        uint64_t p = primes[i];
        uint64_t mod = (uint64_t)((unsigned __int128)x % p);
        if (mod != 3 && mod != (p - 3)) {   /* fails at p */
            fail_prime = p;
            break;
        }
    }
    if (fail_prime == 0) return false;    /* no failure found → not a term */

    /* remove all prime factors smaller than fail_prime from val */
    for (size_t i = 0; i < primes_cnt && primes[i] < fail_prime; ++i)
    {
        uint64_t p = primes[i];
        while ((val % p) == 0) val /= p;
    }

    /* if any factor remains, it is a prime greater than fail_prime,
       thus the condition is violated. */
    return (val == (seq_t)1);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but safe */

    init_primes();

    static seq_t candidate = 5;  /* the smallest odd number that can be a term */
    while (terms_len < n)
    {
        if (candidate_valid(candidate))
        {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = candidate;
        }
        candidate += 2;   /* only odd numbers can satisfy the first prime 2 */
    }

    return terms[n - 1];
}
