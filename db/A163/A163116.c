/*
 * A163116  Partial sums of A161671.
 * https://oeis.org/A163116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of first n primes – sum of first n-1 non‑primes (1
 *            treated as non‑prime)
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Compute the n‑th term of OEIS A163116                               */
/*  Parameters:                                                        */
/*    n – index (offset=1)                                              */
/*  Return: a(n) expressed as seq_t (external to the build)             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but harmless */

    /* Special case n = 1 : a(1) = 2 */
    if (n == 1) return (seq_t)2;

    /* Estimate a limit large enough to contain n primes and n-1 nonprimes. */
    idx_t limit;
    if (n < 6) {
        /* For very small n the 15th number is enough */
        limit = 15;
    } else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn)));
        /* Add a safety margin to be certain the limit covers all needed numbers */
        limit = (idx_t)(est + 5.0 * dn + 10.0);
    }

    /* Allocate a boolean array for the sieve (true = prime).          */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) abort();

    /* Sieve of Eratosthenes */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = true;
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (idx_t multiple = p * p; multiple <= limit; multiple += p)
                is_prime[multiple] = false;
        }
    }

    /* Accumulators */
    seq_t sum_primes      = (seq_t)0;
    seq_t sum_nonprimes   = (seq_t)0;
    idx_t count_primes     = 0;
    idx_t count_nonprimes = 0;

    /* 1 is considered a non‑prime */
    if (n > 1) {
        sum_nonprimes   = (seq_t)1;
        count_nonprimes = 1;
    }

    /* Scan through integers starting from 2 */
    for (idx_t i = 2; i <= limit; ++i) {
        if (count_primes == n && count_nonprimes == n - 1)
            break;

        if (is_prime[i]) {
            if (count_primes < n) {
                sum_primes += (seq_t)i;
                ++count_primes;
            }
        } else {
            if (count_nonprimes < n - 1) {
                sum_nonprimes += (seq_t)i;
                ++count_nonprimes;
            }
        }
    }

    free(is_prime);
    return sum_primes - sum_nonprimes;
}
