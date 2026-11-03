/*
 * A067893  Nonprimes k such that tau(k)*sigma(k) < prime(k) where tau(k) =
 *          A000005(k) and sigma(k) = A000203(k).
 * https://oeis.org/A067893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 530, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9831 (2026-11-01)
 * Helpers:   is_prime, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A067893
   a(n) = the n-th nonprime k such that tau(k)*sigma(k) < prime(k) 
   where tau = number of divisors (A000005), sigma = sum of divisors (A000203),
   and prime(k) is the k-th prime (i.e. prime(1)=2, prime(2)=3, …). */

#include <stddef.h>              /* for size_t */
#include <stdbool.h>

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/num_divisors.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Returns the n-th term of the sequence (indexing starts at 1). */
seq_t a(idx_t n)
{
    /* static state to allow calls in arbitrary order */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cache_len = 0;  /* number of terms stored so far */
    static size_t cache_cap = 0;  /* allocated capacity of cache   */
    static idx_t  cur_k   = 1;    /* next integer to examine */

    /* extend the cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* search for the next qualifying k */
        while (1)
        {
            if (!is_prime(cur_k))          /* only nonprimes are candidates */
            {
                seq_t tau   = num_divisors(cur_k);     /* A000005 */
                seq_t sigma = sigma_of(cur_k);               /* A000203 */
                seq_t prod  = tau * sigma;
                seq_t p_k   = (seq_t)get_prime(cur_k);       /* k‑th prime   */

                if (prod < p_k)           /* condition satisfied */
                {
                    /* enlarge cache if needed */
                    if (cache_len == cache_cap)
                    {
                        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
                        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                        cache    = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = cur_k;   /* store the term      */
                    cur_k++;                     /* move to next candidate */
                    break;                        /* exit the inner loop   */
                }
            }
            cur_k++;                           /* try the next integer   */
        }
    }

    /* return the (n‑1)-th element of zero‑based array */
    return cache[n - 1];
}
