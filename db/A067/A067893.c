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
 * Helpers:   divisor_count_u64, get_prime, is_prime, seq_alloc, seq_realloc,
 *            sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A067893
   a(n) = the n-th nonprime k such that tau(k)*sigma(k) < prime(k) 
   where tau = number of divisors (A000005), sigma = sum of divisors (A000203),
   and prime(k) is the k-th prime (i.e. prime(1)=2, prime(2)=3, …). */

#include <stddef.h>              /* for size_t */
#include <stdbool.h>

#include "../utils/divisor_count_u64.h"  /* seq_t divisor_count_u64(idx_t) */
#include "../utils/sigma_of.h"          /* seq_t sigma_of(idx_t) */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/get_prime.h"         /* uint32_t get_prime(idx_t) */
#include "../utils/seq_alloc.h"         /* seq_t* seq_alloc_new_array(size_t) */
#include "../utils/seq_realloc.h"       /* seq_t* seq_realloc(seq_t*, size_t, size_t) */

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
                seq_t tau   = divisor_count_u64(cur_k);     /* A000005 */
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
