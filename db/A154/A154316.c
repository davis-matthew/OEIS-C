/*
 * A154316  Primes prime(k) such that abs(sum of digits of prime(k) - sum of
 *          digits of k) < 2.
 * https://oeis.org/A154316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum, grow_seq, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th prime p such that |sum_digits(p) - sum_digits(n)| < 2 */

#include <stdlib.h>                 /* for size_t, malloc, realloc */
#include "../utils/grow_seq.h"      /* grow_seq */
#include "../utils/digit_sum.h"     /* digit_sum */
#include "../utils/next_prime.h"    /* next_prime */

seq_t a(idx_t n)
{
    /* Static cache of sequence values */
    static seq_t *found = NULL;
    static size_t found_len = 0;
    static size_t found_cap = 0;

    /* Last prime examined and its index (prime number) */
    static uint64_t last_prime = 0;
    static idx_t last_prime_index = 0;

    if (n == 0)               /* offset is 1, but guard anyway */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (found_len < (size_t)n)
    {
        /* Find next prime and its index */
        if (last_prime == 0)
        {
            last_prime = 2;
            last_prime_index = 1;
        }
        else
        {
            last_prime = next_prime(last_prime);
            last_prime_index++;
        }

        /* Compute digit sums */
        int ds_prime = (int)digit_sum((seq_t)last_prime);
        int ds_index = (int)digit_sum((seq_t)last_prime_index);

        /* Check the condition |sum_digits(p) - sum_digits(n)| < 2 */
        int diff = ds_prime - ds_index;
        if (diff < 0) diff = -diff;
        if (diff < 2)
        {
            /* Add to cache */
            grow_seq(&found, &found_cap, found_len + 1);
            found[found_len] = (seq_t)last_prime;
            found_len++;
        }
    }

    return found[n - 1];
}
