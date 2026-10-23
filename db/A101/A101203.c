/*
 * A101203  a(n) = sum of nonprimes <= n.
 * https://oeis.org/A101203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_of_nonprimes_up_to_n = A000217(n) - A034387(n) (Δ =
 *            sum of primes ≤ n)
 * Range:     u8 n <= 26, u16 n <= 398, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses a cumulative sum of primes cached statically.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computes a(n) = Σ_{k=0..n, k not prime} k   (OEIS A101203) */
seq_t a(idx_t n)
{
    /* Cache of cumulative prime sums: prime_sum[i] = sum of all primes ≤ i   */
    static seq_t *prime_sum = NULL;   /* array of seq_t values, indexed by idx_t */
    static idx_t  max_cached = 0;     /* highest index for which array is valid   */

    /* Expand cache if necessary */
    if (n >= max_cached)
    {
        idx_t new_size = n + 1;       /* indices 0 … n inclusive          */
        seq_t *new_arr = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        if (!new_arr)
            return 0;   /* Allocation failed – return 0 as an arbitrary value */

        /* Simple Eratosthenes sieve on indices 0 … n                           */
        unsigned char *is_prime = (unsigned char *)malloc((size_t)new_size);
        if (!is_prime)
        {
            free(new_arr);
            return 0;
        }

        for (idx_t i = 0; i < new_size; ++i)
            is_prime[i] = 1;
        if (new_size > 0)  is_prime[0] = 0;
        if (new_size > 1)  is_prime[1] = 0;

        for (idx_t p = 2; p * p <= n; ++p)
        {
            if (is_prime[p])
            {
                for (idx_t m = p * p; m <= n; m += p)
                    is_prime[m] = 0;
            }
        }

        /* Build cumulative prime sum */
        seq_t sum = 0;
        new_arr[0] = 0;
        for (idx_t i = 1; i <= n; ++i)
        {
            if (is_prime[i])
                sum += (seq_t)i;     /* cast to seq_t for the addition            */
            new_arr[i] = sum;
        }

        free(is_prime);

        /* Replace old cache if it exists */
        if (prime_sum)
            free(prime_sum);

        prime_sum   = new_arr;
        max_cached  = new_size;
    }

    /* Total sum 0 + 1 + … + n   (computed in seq_t)                         */
    seq_t total_sum = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Sum of primes up to n                                                   */
    seq_t prime_sum_n = prime_sum[n];

    /* Sum of nonprimes   */
    seq_t result = total_sum - prime_sum_n;
    return result;
}
