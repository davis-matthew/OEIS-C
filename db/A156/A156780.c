/*
 * A156780  sp(n)*pi(n) = A034387(n)*A000720(n) = (sum of primes <= n)*(number
 *          of primes <= n).
 * https://oeis.org/A156780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sp(n) * pi(n) where sp(n) = sum of primes ≤ n pi(n) =
 *            number of primes ≤ n
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   build_primes_upto, seq_alloc_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free.h"

seq_t a(idx_t n)
{
    /* Cache of primes and their prefix sums */
    static idx_t       cache_limit = 0;
    static uint64_t   *primes      = NULL;  /* primes up to cache_limit */
    static seq_t      *prefix_sum  = NULL;  /* prefix_sum[i] = sum of first i primes */
    static size_t      prime_cnt   = 0;     /* number of primes stored */

    /* Rebuild cache if n exceeds current limit */
    if (primes == NULL || n > cache_limit)
    {
        size_t new_cnt;
        uint64_t *new_primes;

        /* Build prime list up to n */
        build_primes_upto(n, &new_primes, &new_cnt);

        /* Free old data */
        free(primes);
        if (prefix_sum != NULL)
            seq_free(prefix_sum);

        /* Store new data */
        primes     = new_primes;
        prime_cnt  = new_cnt;
        cache_limit = n;

        /* Allocate prefix sum array */
        prefix_sum = seq_alloc_array(prime_cnt + 1);
        if (prefix_sum == NULL)
            return (seq_t)0;  /* allocation failure */

        /* Initialize prefix sums */
        prefix_sum[0] = (seq_t)0;
        for (size_t i = 0; i < prime_cnt; ++i)
        {
            /* seq_t supports addition with numeric types */
            prefix_sum[i + 1] = prefix_sum[i] + (seq_t)primes[i];
        }
    }

    /* Find pi(n) = number of primes ≤ n by binary search */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) / 2;
        if (primes[mid] <= (uint64_t)n)
            lo = mid + 1;
        else
            hi = mid;
    }
    size_t pi = lo;

    /* sp(n) = sum of primes ≤ n */
    seq_t sp = prefix_sum[pi];

    /* Return product sp(n) * pi(n) */
    return sp * (seq_t)pi;
}
