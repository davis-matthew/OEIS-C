/*
 * A065896  Number of composites <= 2*n.
 * https://oeis.org/A065896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of composite integers ≤ 2·n
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static tables that grow only upward */
    static idx_t   max_limit      = 0;        /* current maximum index (i.e. largest 2·n handled) */
    static seq_t  *composite_cnt  = NULL;     /* cumulative number of composites ≤ i */
    static uint8_t *is_prime      = NULL;     /* 1 = prime, 0 = composite (or 0/1) */

    /* Ensure that everything is computed up to target = 2·n */
    idx_t target = n << 1;                    /* 2 * n, safe for unsigned 64-bit */

    if (target > max_limit)
    {
        /* Allocate new arrays for the larger limit */
        size_t new_size = (size_t)target + 1;   /* include index target itself */

        /* Allocate or reallocate prime flag array */
        uint8_t *new_prime = (uint8_t*)malloc(new_size);
        if (!new_prime) return 0;              /* out of memory: return 0 as a safe fallback */

        /* Initialize all as probable primes */
        for (size_t i = 0; i < new_size; ++i)
            new_prime[i] = 1;
        /* 0 and 1 are not prime */
        if (new_size > 0) new_prime[0] = 0;
        if (new_size > 1) new_prime[1] = 0;

        /* Sieve of Eratosthenes up to 'target' */
        for (idx_t p = 2; p * p <= target; ++p)
        {
            if (new_prime[p])
            {
                for (idx_t m = p * p; m <= target; m += p)
                    new_prime[m] = 0;
            }
        }

        /* Allocate/composite cumulative array */
        seq_t *new_cnt = (seq_t*)malloc(new_size * sizeof(seq_t));
        if (!new_cnt) { free(new_prime); return 0; }

        /* Fill cumulative composite counts */
        seq_t cum = 0;
        for (idx_t i = 0; i <= target; ++i)
        {
            if (i >= 4 && !new_prime[i]) /* composites start at 4 */
                ++cum;
            new_cnt[i] = cum;
        }

        /* Replace old tables with new ones */
        free(is_prime);
        free(composite_cnt);
        is_prime = new_prime;
        composite_cnt = new_cnt;
        max_limit = target;
    }

    /* Return the precomputed answer */
    return composite_cnt[target];
}
