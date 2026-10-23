/*
 * A139119  Primes whose binary representation shows the distribution of prime
 *          numbers using "0" for primes and "1" for nonprime numbers.
 * https://oeis.org/A139119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   For k = 1,2,... construct S_k = a(1)a(2)...a(k) where a(i) is
 *            the characteristic function of non‑prime numbers (1 if i is
 *            composite or 1, 0 if i is prime). a(n) is the nth prime value of
 *            S_k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function uses the provided utilities:
   is_prime_small(seq_t)  - primality test for small integers
   is_prime_big(seq_t)    - primality test for arbitrary precision */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Static cache of found primes */
    static seq_t *cache   = NULL;   /* array of prime values */
    static size_t cache_size   = 0; /* number of primes cached */
    static size_t cache_cap    = 0; /* allocated capacity of cache   */

    /* State of the incremental construction of S_k */
    static idx_t k         = 0;     /* current length of S_k (k >= 0) */
    static seq_t cur_val   = 0;     /* current value of S_k as a sequence integer */

    /* Lazy allocation and growth of the cache array */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
    }

    /* Extend the cache until it contains at least n terms */
    while (cache_size < (size_t)n)
    {
        /* Advance k and append the next bit a(k+1) */
        ++k;
        bool bit = !is_prime_small((seq_t)k); /* 1 if k is composite (or 1), 0 if prime */
        seq_t shifted = cur_val << 1;
        if (bit) shifted |= (seq_t)1;
        cur_val = shifted;

        /* Test the new value for primality */
        if (is_prime_big(cur_val))
        {
            /* Grow cache if necessary */
            if (cache_size >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
            }
            cache[cache_size++] = cur_val;
        }
    }

    /* Cache is 0‑based but n is 1‑based */
    return cache[n - 1];
}
