/*
 * A124786  Numbers n such that n, sopfr(n) and sopfr(sopfr(n)) are all
 *          semiprimes.
 * https://oeis.org/A124786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124786: numbers n such that n, sopfr(n), sopfr(sopfr(n)) are all
   semiprimes.  The function performs a growing search; no closed formula is
   known. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Integer square root: floor(sqrt(n)). */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t low = 1, high = n / 2, mid, ans = 1;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (mid <= n / mid)
        {
            ans = mid;
            low = mid + 1;
        }
        else
            high = mid - 1;
    }
    return ans;
}

/* Ensure that 'primes' contains all primes up to 'limit'. */
static void ensure_primes_upto(uint64_t limit,
                               uint64_t **primes, size_t *cnt, uint64_t *cur_limit)
{
    if (limit <= *cur_limit)
        return;
    uint64_t *newpr;
    size_t newcnt;
    build_primes_upto(limit, &newpr, &newcnt);
    if (*primes) free(*primes);
    *primes = newpr;
    *cnt    = newcnt;
    *cur_limit = limit;
}

/* Test whether n is a semiprime (product of exactly two primes,
   counting multiplicity). */
static bool is_semiprime_u64(uint64_t n,
                             uint64_t *pr, size_t pc)
{
    if (n < 2) return false;
    int fcnt = 0;
    uint64_t t = n;
    for (size_t i = 0; i < pc && pr[i] * pr[i] <= t; ++i)
    {
        uint64_t p = pr[i];
        while (t % p == 0)
        {
            ++fcnt;
            if (fcnt > 2) return false;
            t /= p;
        }
    }
    if (t > 1) ++fcnt;
    return fcnt == 2;
}

/* Sum of prime factors with multiplicity (sopfr). */
static uint64_t sopfr_u64(uint64_t n,
                          uint64_t *pr, size_t pc)
{
    uint64_t sum = 0;
    uint64_t t = n;
    for (size_t i = 0; i < pc && pr[i] * pr[i] <= t; ++i)
    {
        uint64_t p = pr[i];
        while (t % p == 0)
        {
            sum += p;
            t /= p;
        }
    }
    if (t > 1) sum += t;
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1; a(0) unused */

    /* Static dynamic storage for the sequence. */
    static seq_t *list   = NULL;
    static size_t len    = 0;
    static size_t cap    = 0;

    /* Static storage for prime generation. */
    static uint64_t last_candidate = 1;      /* next integer to test */
    static uint64_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static uint64_t prime_lim = 0;

    /* Generate terms up to the requested index. */
    while (len < (size_t)n)
    {
        /* Search for the next qualifying integer. */
        for (uint64_t cand = last_candidate + 1;; ++cand)
        {
            /* Ensure primes up to sqrt(cand) are available. */
            uint64_t need_cand = isqrt_u64(cand);
            ensure_primes_upto(need_cand, &prime_arr, &prime_cnt, &prime_lim);

            if (!is_semiprime_u64(cand, prime_arr, prime_cnt))
                continue;

            uint64_t s1 = sopfr_u64(cand, prime_arr, prime_cnt);
            /* Ensure primes up to sqrt(s1). */
            uint64_t need_s1 = isqrt_u64(s1);
            ensure_primes_upto(need_s1, &prime_arr, &prime_cnt, &prime_lim);
            if (!is_semiprime_u64(s1, prime_arr, prime_cnt))
                continue;

            uint64_t s2 = sopfr_u64(s1, prime_arr, prime_cnt);
            /* Ensure primes up to sqrt(s2). */
            uint64_t need_s2 = isqrt_u64(s2);
            ensure_primes_upto(need_s2, &prime_arr, &prime_cnt, &prime_lim);
            if (!is_semiprime_u64(s2, prime_arr, prime_cnt))
                continue;

            /* Record the found term. */
            last_candidate = cand;
            if (len == cap)
            {
                cap = cap ? cap * 2 : 16;
                list = (seq_t *)realloc(list, cap * sizeof(seq_t));
            }
            list[len++] = (seq_t)cand;
            break;
        }
    }
    return list[n - 1];
}
