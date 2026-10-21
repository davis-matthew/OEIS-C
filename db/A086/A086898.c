/*
 * A086898  a(n) = Sum_{d|n} tau(d-1).
 * https://oeis.org/A086898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} tau(d-1) where tau(k) is the divisor count of k
 *            (tau(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *tau = NULL;          /* divisor counts */
    static idx_t tau_size = 0;         /* currently allocated size */

    /* No need to support n==0, but handle gracefully */
    if (n == 0) return 0;

    /* Ensure tau array covers indices 0 .. n-1 (needed for tau(d-1)) */
    if (n > tau_size)
    {
        idx_t new_size = n;                /* need at least indices 0..n-1 */
        if (new_size < 2) new_size = 2;    /* protect against tiny sizes */

        seq_t *tmp = (seq_t *)realloc(tau, (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure: return 0 as safe */
        tau = tmp;

        /* Initialize all entries to 0 */
        for (idx_t i = 0; i < new_size; ++i)
            tau[i] = 0;

        /* Sieve-like computation of divisor counts tau[1..new_size-1] */
        for (idx_t i = 1; i < new_size; ++i)
        {
            for (idx_t j = i; j < new_size; j += i)
                tau[j] += 1;
        }

        tau_size = new_size;
    }

    /* Sum tau(d-1) for all divisors d of n */
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            /* divisor i */
            idx_t d1 = i;
            if (d1 > 0)           /* d1-1 is in range */
                sum += tau[d1 - 1];
            /* divisor n/i, avoid double counting when i == n/i */
            idx_t d2 = n / i;
            if (d2 != d1 && d2 > 0)
                sum += tau[d2 - 1];
        }
    }

    return sum;
}
