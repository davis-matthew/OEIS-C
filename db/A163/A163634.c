/*
 * A163634  a(n) = (p*(p+4)+1)/2 where (p,p+4) are the n-th cousin prime pair.
 * https://oeis.org/A163634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p*(p+4)+1)/2 where (p,p+4) is the n‑th cousin prime pair
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Offset is 1.  For n==0 the behaviour is irrelevant. */
    if (n == 0) return 0;

    /* Static storage for the found cousin‑prime first members and the
       number of pairs known so far.  They persist across calls. */
    static uint64_t *cprimes = NULL;   /* array of primes p where p and p+4 are primes */
    static idx_t     ccount  = 0;      /* number of such pairs stored in cprimes */

    /* If we don't yet have the n‑th pair, extend the sieve until we do. */
    if (ccount < n) {
        /* If we had a previous allocation we free it – we recompute from scratch
           for the new (larger) limit. */
        if (cprimes) { free(cprimes); cprimes = NULL; }

        idx_t limit = 100000;          /* start with a modest limit */
        while (1) {
            /* Allocate the sieve array.  A single byte per number is
               adequate and simple. */
            bool *is_prime = (bool *)malloc((size_t)(limit + 1));
            if (!is_prime) return 0;   /* allocation failed – return 0 */

            /* Initialise the sieve. */
            for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
            if (limit >= 0) is_prime[0] = false;
            if (limit >= 1) is_prime[1] = false;

            /* Classic Eratosthenes sieve. */
            for (idx_t i = 2; i * i <= limit; ++i) {
                if (is_prime[i]) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_prime[j] = false;
                }
            }

            /* Count how many cousin‑prime pairs we have up to this limit. */
            idx_t pair_cnt = 0;
            for (idx_t p = 2; p + 4 <= limit; ++p)
                if (is_prime[p] && is_prime[p + 4])
                    ++pair_cnt;

            /* If this limit already gives us enough pairs, store them
               and terminate. */
            if (pair_cnt >= n) {
                cprimes = (uint64_t *)malloc((size_t)pair_cnt * sizeof(uint64_t));
                if (!cprimes) {
                    free(is_prime);
                    return 0;           /* allocation failed */
                }
                idx_t idx = 0;
                for (idx_t p = 2; p + 4 <= limit; ++p)
                    if (is_prime[p] && is_prime[p + 4])
                        cprimes[idx++] = p;
                ccount = pair_cnt;
                free(is_prime);
                break;
            }

            /* We need more pairs – free the sieve and double the limit. */
            free(is_prime);
            limit <<= 1;
        }
    }

    /* Retrieve the appropriate first prime p and compute the term. */
    uint64_t p = cprimes[n - 1];
    seq_t   val = (seq_t)p * ((seq_t)p + 4) + 1; /* (p*(p+4)+1) */
    val /= 2;                                    /* divide by 2 */
    return val;
}
