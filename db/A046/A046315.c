/*
 * A046315  Odd semiprimes: odd numbers divisible by exactly 2 primes (counted
 *          with multiplicity).
 * https://oeis.org/A046315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A046315 – odd semiprimes (odd numbers with exactly two prime
 *  factors, counting multiplicity).  The function below computes
 *  a(n) (1‑based index) and memoises the results.
 */
/*  All arithmetic is on seq_t.  The helper function
 *  floor_isqrt_u64 is used to avoid overflow when checking
 *  p*q <= bound.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/floor_isqrt_u64.h"     /* floor_isqrt_u64 */
#include "../utils/build_primes_upto.h"   /* build_primes_upto */

/* comparison for qsort */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

/* cache of odd semiprimes */
static seq_t *sem_cache = NULL;
static size_t sem_len   = 0;     /* number of cached values     */
static idx_t bound      = 0;     /* max value used when cache was built */

/* ensure that the cache contains at least 'need' elements, growing
 * the bound until this is satisfied.  The function rebuilds the
 * cache from scratch each time it is enlarged.  For the problem
 * sizes this is fast enough while keeping the code simple.
 */
static void ensure_cache(idx_t need)
{
    if (need <= sem_len)
        return;

    /* start with a small bound; grow exponentially until enough */
    idx_t new_bound = bound ? bound : 1000;

    while (sem_len < need) {
        /* double the bound */
        if (new_bound == 0)
            new_bound = 1000;
        else
            new_bound <<= 1;

        /* build the list of primes up to new_bound */
        size_t prime_cnt = 0;
        uint64_t *primes = NULL;
        build_primes_upto(new_bound, &primes, &prime_cnt);

        /* temporary array for possible products */
        size_t alloc = prime_cnt * (prime_cnt + 1) / 2 + 10;
        seq_t *tmp = (seq_t *)malloc(alloc * sizeof(seq_t));
        size_t cnt = 0;

        /* pre‑compute floor(sqrt(new_bound)) */
        uint64_t lim_sq = floor_isqrt_u64(new_bound);

        /* collect odd semiprimes <= new_bound */
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p == 2)          /* skip the even prime */
                continue;

            for (size_t j = i; j < prime_cnt; ++j) {
                uint64_t q = primes[j];
                if (q == 2)
                    continue;

                /* quick bound: if both > sqrt(new_bound) then product > bound */
                if (p > lim_sq && q > p)
                    break;

                unsigned __int128 prod = (unsigned __int128)p * q;
                if (prod > (unsigned __int128)new_bound)
                    break;

                tmp[cnt++] = (seq_t)prod;
            }
        }

        free(primes);

        /* sort the new list */
        qsort(tmp, cnt, sizeof(seq_t), cmp_seq_t);

        /* replace old cache */
        free(sem_cache);
        sem_cache = tmp;
        sem_len   = cnt;
        bound     = new_bound;
    }
}

/* return the n‑th odd semiprime (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined; guard against zero */
        return (seq_t)0;

    ensure_cache(n);
    return sem_cache[n - 1];
}
