/*
 * A031395  a(n) = prime(8*n - 4).
 * https://oeis.org/A031395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(8*n - 4) (the (8*n-4)-th prime)
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes.h"

/* Estimate an upper bound for the k-th prime using the prime number theorem.
   For k < 6 we return 15, otherwise p_k <= k (log k + log log k) + 3k.
   The returned value is always an integer. */
static idx_t estimate_limit(idx_t k)
{
    if (k < 6) return 15;
    double dk = (double)k;
    double lim = dk * (log(dk) + log(log(dk))) + 3.0 * dk;
    return (idx_t)lim + 1;  /* add 1 to be safe */
}

seq_t a(idx_t n)
{
    /* static cache of primes */
    static idx_t *prime_arr = NULL;
    static size_t prime_cnt = 0;
    static idx_t sieve_lim = 0;

    /* k = 8*n - 4  (1‑based index of the required prime) */
    idx_t k = n * 8 - 4;
    if (k < 1) k = 1;   /* safety for n = 0, though offset is 1 */

    /* If we don't have enough primes cached, recompute with a larger limit */
    if (prime_cnt < (size_t)k) {
        /* Determine a new upper bound that will contain at least k primes */
        idx_t new_lim = estimate_limit(k);
        /* If we already had a limit larger than needed, keep it */
        if (sieve_lim >= new_lim) new_lim = sieve_lim;

        /* Sieve until we have enough primes.  The upper bound is monotonic,
           so we only increase it if necessary. */
        while (1) {
            free(prime_arr);          /* free old result */
            size_t cnt;
            build_primes(new_lim, &prime_arr, &cnt);
            prime_cnt = cnt;
            sieve_lim = new_lim;
            if (prime_cnt >= (size_t)k) break;
            /* not enough primes, double the limit and try again */
            new_lim *= 2;
        }
    }

    /* Return the k-th prime (k is 1‑based) */
    return (seq_t)prime_arr[k - 1];
}
