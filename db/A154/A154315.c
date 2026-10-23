/*
 * A154315  Numbers k such that the number of prime digits in prime(k) equals
 *          the number of even digits in prime(k).
 * https://oeis.org/A154315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the n‑th index k such that the number of prime digits
 *            (2,3,5,7) in the k‑th prime equals the number of even digits in
 *            that prime. The sequence is the list of such indices k
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
/* ≥ C11 and C++17 */ 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

/* Memory helpers that work in both C and C++.  They allocate/​free
   arrays of seq_t elements, taking care of constructors in C++. */
#ifdef __cplusplus
#include <new>
static seq_t *arr_alloc(size_t n)   { return new seq_t[n]; }
static void   arr_free(seq_t *p)    { delete[] p; }
#else
static seq_t *arr_alloc(size_t n)   { return (seq_t *)malloc(n * sizeof(seq_t)); }
static void   arr_free(seq_t *p)    { free(p); }
#endif

/* Count prime digits in prime and even digits in prime. */
static bool prime_even_equal(uint64_t p)
{
    int prime_cnt = 0;
    int even_cnt  = 0;
    while (p)
    {
        int d = (int)(p % 10);
        if (d == 2 || d == 3 || d == 5 || d == 7) ++prime_cnt;
        if ((d & 1) == 0)                         ++even_cnt;
        p /= 10;
    }
    return prime_cnt == even_cnt;
}

/* Prime storage: dynamic array of uint64_t storing the primes discovered
   so far.  The array is filled incrementally up to the required index
   when needed. */
static uint64_t *primes = NULL;
static size_t   prime_cnt = 0;
static size_t   prime_cap = 0;
static uint64_t last_checked = 1;   /* last candidate that was tested */

/* Ensure that primes array contains at least <count> primes. */
static void ensure_primes_at_least(size_t count)
{
    while (prime_cnt < count)
    {
        uint64_t cand = last_checked + 1;
        bool isprime = true;
        uint64_t lim = (uint64_t)int_sqrt(cand);
        for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i)
        {
            if (cand % primes[i] == 0) { isprime = false; break; }
        }
        if (isprime)
        {
            if (prime_cnt == prime_cap)
            {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes,
                                                    prime_cap * sizeof(uint64_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[prime_cnt++] = cand;
        }
        last_checked = cand;
    }
}

/* Cached answer list: a dynamic array of seq_t values containing the
   indices k that satisfy the property, up to the largest n requested
   so far. */
static seq_t *cache = NULL;
static size_t cache_sz  = 0;
static size_t cache_cap = 0;
static idx_t  last_k_checked = 0;   /* largest k already examined */

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Grow cache as needed. */
    while (cache_sz < (size_t)n)
    {
        /* Examine next k. */
        ++last_k_checked;
        /* Ensure we know the kth prime. */
        ensure_primes_at_least((size_t)last_k_checked);
        uint64_t pk = primes[(size_t)last_k_checked - 1];
        /* Check the condition. */
        if (prime_even_equal(pk))
        {
            if (cache_sz + 1 > cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = arr_alloc(cache_cap);
                if (!tmp) abort();
                for (size_t i = 0; i < cache_sz; ++i) tmp[i] = cache[i];
                if (cache) arr_free(cache);
                cache = tmp;
            }
            cache[cache_sz++] = (seq_t)last_k_checked;
        }
    }
    return cache[(size_t)n - 1];
}
