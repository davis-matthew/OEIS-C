/*
 * A130090  Primes prime(n) such that at least one of the two numbers
 *          (prime(n+1)^2-prime(n)^2)/2 - 1 and (prime(n+1)^2-prime(n)^2)/2 +
 *          1 is not prime.
 * https://oeis.org/A130090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A130090: a(n) is the n-th prime p for which at least one of  
   ((prime(n+1)^2 - prime(n)^2)/2 - 1) or ((prime(n+1)^2 - prime(n)^2)/2 + 1)  
   is composite.  This function builds the sequence lazily and memoises
   results so repeated calls are fast.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

/* Return the n-th term of A130090 (1‑based).  */
seq_t a(idx_t n)
{
    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;           /* array of qualifying primes   */
    static size_t cache_len = 0;          /* number of elements in cache */
    static size_t cache_cap = 0;          /* allocated capacity          */

    /* Static state for continued prime generation between calls. */
    static seq_t prev_prime = 0;          /* prime(n) from last iteration */
    static idx_t  current_k  = 0;         /* index of prev_prime (prime(current_k)) */
    static bool    first_time = true;     /* initialise on first call */

    /* Grow or initialise the cache array on first use. */
    if (first_time) {
        first_time = false;
        /* Prime 1 is 2, prime 2 is 3, etc.  */
        prev_prime = (seq_t)get_prime(1);   /* p(1) = 2 */
        current_k  = 1;
    }

    /* Expand the required number of cache entries.  */
    size_t target = (size_t)n;
    if (cache_len < target) {
        /* Ensure the cache can grow.  */
        if (cache_cap == 0) {
            cache_cap = 16;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        while (cache_len < target) {
            /* Get the next prime.  */
            ++current_k;                       /* prime index for next.   */
            seq_t curr_prime = (seq_t)get_prime(current_k);

            /* Compute (curr^2 - prev^2) / 2 if divisible by 2.  */
            seq_t diff = curr_prime * curr_prime - prev_prime * prev_prime;
            if ((diff % 2) == 0) {
                seq_t v = diff / 2;
                seq_t v_minus = v - 1;
                seq_t v_plus  = v + 1;
                if (!is_prime_big(v_minus) || !is_prime_big(v_plus)) {
                    /* prev_prime satisfies the condition.  */
                    if (cache_len + 1 > cache_cap) {
                        cache_cap *= 2;
                        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                    }
                    cache[cache_len++] = prev_prime;
                }
            }
            /* Slide window: prev becomes curr for next pair.  */
            prev_prime = curr_prime;
        }
    }

    /* Return the requested term.  n is 1‑based.  */
    return cache[target - 1];
}
