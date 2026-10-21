/*
 * A240087  Smallest difference to start a prime arithmetic progression of
 *          three or more terms with the n-th prime number.
 * https://oeis.org/A240087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 4937, u32 n <= 6262, u64 n <= 8608,
 *            u128 n <= 8608, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the smallest positive integer d such that
 *   prime(n) + d*k  (k = 0,1,2,…) are all prime for at least three terms,
 *   but prime(n) − d is not prime.
 * This code searches d incrementally, caching results for efficiency.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /*
     * Cache: index 0 and 1 are unused; a(0)=a(1)=0.
     * The cache is resized to n+1 on first access to a(n).
     */
    static seq_t *cache = NULL;      /* array of computed a(n) */
    static size_t cache_cap = 0;     /* current size of cache array */

    if (n < 2) {
        /* undefined for n < 2; return 0 for safety */
        return 0;
    }

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            abort(); /* allocation failed */
        }
        /* Initialize newly allocated slots to 0 (sentinel) */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Return cached value if already computed */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* Compute the n-th prime (type seq_t) */
    seq_t prime_n = (seq_t)get_prime(n);
    seq_t answer = 0;

    /* Search incrementally for the smallest suitable difference */
    for (seq_t d = 1; ; ++d) {

        /* If prime_n - d is a prime, skip this d.
           For d > prime_n-2, prime_n-d <= 1, which is never prime. */
        if (d <= prime_n - (seq_t)2 && is_prime_big(prime_n - d)) {
            continue;
        }

        seq_t p1 = prime_n + d;
        if (!is_prime_big(p1)) {
            continue;
        }

        seq_t p2 = prime_n + (seq_t)2 * d;
        if (!is_prime_big(p2)) {
            continue;
        }

        /* Found the smallest d with at least three terms in the progression */
        answer = d;
        break;
    }

    cache[n] = answer;
    return answer;
}
