/*
 * A065244  Primes of form p = 2 + Sum_{k = 1..m} k^2 for some m.
 * https://oeis.org/A065244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + Σ_{k=1..m} k² where the resulting number is prime
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 13, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function finds successive values of m that produce primes and
   caches the primes.  It uses a dynamic array, growing it as needed. */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/is_prime_big.h"

/* Cache of found primes. */
static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;   /* allocated capacity */
static size_t cache_size  = 0;   /* number of primes stored */
static idx_t  last_m      = 0;   /* last m processed */

/* Return the nth prime of the form 2 + Σk² (n >= 1).  */
seq_t a(idx_t n)
{
    /* Grow the cache until it has at least n primes. */
    while (cache_size < n)
    {
        /* Next candidate m. */
        last_m++;
        idx_t m = last_m;

        /* Σk² = m*(m+1)*(2m+1)/6. */
        idx_t sumsq = m * (m + 1) * (2 * m + 1) / 6;
        idx_t cand  = 2 + sumsq;

        /* If the candidate exceeds what can be represented in seq_t,
           stop expanding further because all later candidates are larger. */
        if (cand > (idx_t)((seq_t)-1))
            break;

        seq_t val = (seq_t)cand;

        /* Test for primality. */
        if (is_prime_big(val))
        {
            /* Expand cache if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 8 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(*tmp));
                if (!tmp) { /* allocation failed – abort silently */ return 0; }
                if (prime_cache)
                {
                    memcpy(tmp, prime_cache, cache_size * sizeof(*tmp));
                    free(prime_cache);
                }
                prime_cache = tmp;
                cache_cap   = new_cap;
            }

            prime_cache[cache_size++] = val;
        }
    }

    if (n == 0 || cache_size == 0) return 0;
    return prime_cache[n - 1];
}
