/*
 * A141861  Primes of the form (prime(2*k)-prime(k))/2*2, where prime(k) is
 *          the k-th prime.
 * https://oeis.org/A141861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 277, u32 n <= 641, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, is_prime_big, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A141861.
   a(n) = (p(2*k) - p(k)) / 4, where p(j) is the j‑th prime
   and the result is prime.  The sequence is generated lazily
   and cached for repeated calls. */
#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h" /* bool is_prime_big(seq_t n); */
#include "../utils/seq_realloc.h"  /* seq_t* seq_realloc(seq_t*, size_t, size_t); */

seq_t a(idx_t n)
{
    /* static cache of the sequence values */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored     */
    static size_t cache_cap  = 0;   /* allocated capacity         */
    static idx_t  last_k     = 0;   /* last k processed            */

    /* Ensure that we have at least n terms cached */
    while (cache_len < (size_t)n)
    {
        /* Next k to examine */
        last_k++;

        /* Retrieve the k‑th and 2k‑th primes */
        uint32_t pk  = get_prime((idx_t)last_k);
        uint32_t p2k = get_prime((idx_t)(last_k * 2));

        /* Compute difference and check divisibility by 4 */
        seq_t diff = (seq_t)p2k - (seq_t)pk;
        if ((diff % 4) != 0)
            continue;            /* not divisible by 4 */

        /* Value of the candidate prime */
        seq_t val = diff / 4;    /* integer division by 4 */

        /* Test primality */
        if (is_prime_big(val))
        {
            /* Grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                cache = seq_realloc(cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    /* Return the requested term (1‑based index) */
    return cache[n - 1];
}
