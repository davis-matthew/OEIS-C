/*
 * A141063  a(n) = n mod (sum of digits of prime(n)).
 * https://oeis.org/A141063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = n mod (sum of digits of prime(n))
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    static idx_t *primes   = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    if (n == 0u) return 0;  /* offset starts at 1 */

    /* Ensure we have at least n primes */
    while (primes_cnt < (size_t)n)
    {
        idx_t new_limit;

        /* Start from 20 if first time, otherwise double the current limit */
        if (primes_limit == 0)
            new_limit = 20;
        else
            new_limit = primes_limit * 2;

        /* Make sure the new limit is large enough for the n‑th prime
           (an upper bound p_n < n·20 suffices for n ≤ 10 000 and is
           more than enough for typical compilation sizes). */
        if (new_limit < n * 20)
            new_limit = n * 20;

        free(primes);
        primes = NULL;
        primes_cnt = 0;
        build_primes(new_limit, &primes, &primes_cnt);
        primes_limit = new_limit;
    }

    /* nth prime (1‑based indexing) */
    idx_t p = primes[n - 1];

    /* sum of digits of p */
    seq_t sum = 0;
    idx_t t = p;
    while (t != 0)
    {
        sum += (seq_t)(t % 10);
        t /= 10;
    }

    /* n mod sum_of_digits(p) */
    return (seq_t)n % sum;
}
