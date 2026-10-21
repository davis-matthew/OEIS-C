/*
 * A085647  Semiprimes whose prime factors p*q have an equal number of decimal
 *          digits and p is not equal to q.
 * https://oeis.org/A085647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the n-th smallest number that is the product of two
 *            distinct primes p and q such that p and q have the same number
 *            of decimal digits. The sequence is sorted increasingly
 * Range:     u8 none, u16 n <= 2, u32 n <= 9748, u64 n <= 9748,
 *            u128 n <= 9748, big any n
 * Helpers:   build_primes_upto, digit_count, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper functions provided by the infrastructure */
#include "../utils/digit_count.h"
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

/* Compare two seq_t values for qsort.  Works for any seq_t type. */
static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Cached list of semiprimes, along with its size and the prime limit
   used to build it. */
static seq_t  *cache      = NULL;   /* sorted array of products   */
static size_t  cache_size = 0;      /* number of terms in cache   */
static idx_t   cache_limit = 0;     /* upper bound used to generate primes */

/* Ensure that the cache contains at least 'need' terms.  The function
   expands the prime limit by doubling until the required number of
   qualifying semiprimes has been generated, then rebuilds the entire
   cache. */
static void ensure_cache(idx_t need)
{
    if (need == 0 || cache_size >= need) return;

    /* Start with a reasonable prime limit and grow until enough terms. */
    idx_t limit = (cache_limit > 0) ? cache_limit : 1000;
    while (true)
    {
        uint64_t *primes;
        size_t   cnt;
        /* Build primes up to 'limit'. */
        build_primes_upto(limit, &primes, &cnt);
        if (!primes) break;   /* unlikely; abort */

        /* Estimate how many products of same-digit primes we will get. */
        size_t total = 0;
        for (size_t i = 0; i < cnt; )
        {
            size_t j = i;
            int d = digit_count(primes[i]);
            while (j < cnt && digit_count(primes[j]) == d) ++j;
            size_t sz = j - i;
            total += sz * (sz - 1) / 2;
            i = j;
        }
        free(primes);

        if (total >= (size_t)need) break;
        limit <<= 1;   /* double the limit and try again */
    }

    /* Build the final list of products. */
    uint64_t *primes;
    size_t   prime_cnt;
    build_primes_upto(limit, &primes, &prime_cnt);
    if (!primes) return;

    /* Count exact number of products to allocate the array. */
    size_t prod_cnt = 0;
    for (size_t i = 0; i < prime_cnt; )
    {
        size_t j = i;
        int d = digit_count(primes[i]);
        while (j < prime_cnt && digit_count(primes[j]) == d) ++j;
        size_t sz = j - i;
        prod_cnt += sz * (sz - 1) / 2;
        i = j;
    }

    seq_t *arr = (seq_t *)malloc(prod_cnt * sizeof(seq_t));
    if (!arr)
    {
        free(primes);
        return;
    }

    /* Generate all qualifying semiprime products. */
    size_t pos = 0;
    for (size_t i = 0; i < prime_cnt; )
    {
        size_t j = i;
        int d = digit_count(primes[i]);
        while (j < prime_cnt && digit_count(primes[j]) == d) ++j;

        for (size_t p = i; p < j; ++p)
        {
            for (size_t q = p + 1; q < j; ++q)
            {
                /* Use seq_mul if available; it works for arbitrary‑precision
                   seq_t.  For fixed width types it performs modular
                   multiplication, but the products that fit in seq_t will
                   be correct. */
                arr[pos++] = seq_mul((seq_t)primes[p], (seq_t)primes[q]);
            }
        }
        i = j;
    }

    /* Sort the product list. */
    qsort(arr, pos, sizeof(seq_t), cmp_seq_t);

    /* Replace old cache with the new one. */
    free(cache);
    cache = arr;
    cache_size = pos;
    cache_limit = limit;

    free(primes);
}

/* Return the n-th term (n ≥ 1) of sequence A085647. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below offset */
    ensure_cache(n);
    return (n <= cache_size) ? cache[n - 1] : 0;
}
