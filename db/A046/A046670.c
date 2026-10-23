/*
 * A046670  Partial sums of A006530.
 * https://oeis.org/A046670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = sum_{k=1}^n A006530(k) * where A006530(k) is the largest
 *            prime factor of k (with A006530(1)=1)
 * Range:     u8 n <= 31, u16 n <= 672, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for the partial sums */
    static seq_t *sum   = NULL;   /* sum[i] = a(i) */
    static idx_t max_n = 0;       /* maximum n for which sum is computed */

    /* No value defined for n == 0 but return 0 to keep behaviour sane. */
    if (n == 0)
        return 0;

    /* If we have not yet computed up to n, extend the cache. */
    if (n > max_n)
    {
        size_t new_max = (size_t)n;                 /* safe for n <= 10000 */

        /* Reallocate the sum array.  C++ requires an explicit cast. */
        seq_t *new_sum = (seq_t*)realloc(sum, (new_max + 1) * sizeof(seq_t));
        if (new_sum == NULL)
            return 0;  /* allocation failed – honour the contract by aborting */

        sum = new_sum;

        /* Allocate array for the largest prime factor of each number. */
        seq_t *lpf = (seq_t*)calloc(new_max + 1, sizeof(seq_t));
        if (lpf == NULL)
            return 0;  /* allocation failed */

        /* Sieve to fill lpf[j] with the largest prime factor of j. */
        for (size_t i = 2; i <= new_max; ++i)
        {
            if (lpf[i] == 0)            /* i is prime */
            {
                for (size_t j = i; j <= new_max; j += i)
                    lpf[j] = (seq_t)i;   /* set largest prime factor found so far */
            }
        }
        lpf[1] = (seq_t)1; /* definition for A006530(1) */

        /* Compute cumulative sums. */
        sum[0] = 0;
        for (size_t i = 1; i <= new_max; ++i)
            sum[i] = sum[i-1] + lpf[i];

        /* Clean up */
        free(lpf);
        max_n = new_max;
    }

    return sum[n];
}
