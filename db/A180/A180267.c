/*
 * A180267  Prime(2*n+1)!/(60*prime(2*n-1)!).
 * https://oeis.org/A180267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(2*n+1)! / (60 * prime(2*n-1)!)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 11, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (prime(k) is the k‑th prime, primes indexed starting at 1) */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(0) is unused; return 0 for safety */
    if (n == 0) return (seq_t)0;

    /* indices of the required primes */
    idx_t idx_small = 2 * n - 1;   /* prime(2*n-1) */
    idx_t idx_large = 2 * n + 1;   /* prime(2*n+1) */

    /* obtain the prime values */
    seq_t small_prime = (seq_t)get_prime(idx_small);
    seq_t large_prime = (seq_t)get_prime(idx_large);

    /* compute product of integers in (small_prime, large_prime] and divide by 60 */
    seq_t result = (seq_t)1;

    /* factors of 60: 2^2 * 3 * 5 */
    seq_t factors[3] = { (seq_t)2, (seq_t)3, (seq_t)5 };
    int  counts  [3] = { 2,      1,      1      };

    /* loop over the integers between the two primes */
    idx_t start = (idx_t)small_prime + 1;
    idx_t end   = (idx_t)large_prime;

    for (idx_t i = start; i <= end; ++i)
    {
        result *= (seq_t)i;

        /* as soon as possible divide by the remaining factors of 60 */
        for (int j = 0; j < 3; ++j)
        {
            while (counts[j] > 0 && result % factors[j] == 0)
            {
                result /= factors[j];
                --counts[j];
            }
        }
    }

    /* all factors of 60 should have been removed */
    return result;
}
