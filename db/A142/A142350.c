/*
 * A142350  Nonprimes of the form n/4*(c(n)-r(n)), where c(n)=n-th composite
 *          and r(n)=n-th nonprime.
 * https://oeis.org/A142350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(k) = n / (4 * (c(n) - r(n))) (integer) * where c(n) = n-th
 *            composite number * r(n) = n-th nonprime number * Sequence lists
 *            the values of a(k) that are non-prime, in order * of increasing
 *            n (the n that gives the k-th term)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Use the prime test from the utilities. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* Sieve state */
static bool *is_prime     = NULL;   /* true if i is prime */
static uint64_t *comps    = NULL;   /* array of composite numbers */
static idx_t   comps_cnt  = 0;      /* number of composites allocated */
static idx_t   sieve_limit = 0;     /* current size of sieve */
static bool    built      = false;

/* Forward prototype */
static void rebuild_sieve(idx_t limit);

/* Build or rebuild the sieve up to 'limit' and record composites. */
static void rebuild_sieve(idx_t limit)
{
    free(is_prime);
    free(comps);

    sieve_limit = limit;
    is_prime = (bool *)malloc((size_t)(sieve_limit + 1));
    if (!is_prime) exit(EXIT_FAILURE);

    /* Eratosthenes */
    for (idx_t i = 0; i <= sieve_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= sieve_limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= sieve_limit; m += p)
                is_prime[m] = false;

    /* Record composites */
    comps = (uint64_t *)malloc((size_t)sieve_limit * sizeof(uint64_t));
    if (!comps) exit(EXIT_FAILURE);
    comps_cnt = 0;
    for (idx_t x = 4; x <= sieve_limit; ++x)
        if (!is_prime[x]) comps[comps_cnt++] = (uint64_t)x;
}

/* Ensure we have at least 'needed' composites recorded. */
static void ensure_composites(idx_t needed)
{
    while (needed > comps_cnt) {
        idx_t new_lim = sieve_limit * 2;
        if (new_lim < sieve_limit + 1000) new_lim = sieve_limit + 1000;
        rebuild_sieve(new_lim);
    }
}

/* Core function: compute the n-th term of A142350. */
seq_t a(idx_t n)
{
    /* Initialise once */
    if (!built) {
        const idx_t INITIAL = 2000000;   /* enough for n up to ~10 000 */
        rebuild_sieve(INITIAL);
        built = true;
    }

    idx_t found = 0;          /* terms produced so far */
    idx_t current_n = 1;      /* n used in the formula */

    while (true) {
        /* Only n divisible by 4 can produce an integer result. */
        if (current_n % 4 == 0 && current_n >= 3) {
            ensure_composites(current_n);

            /* c(n) = comps[n-1] */
            uint64_t c_n = comps[current_n - 1];
            /* r(n) = 1 if n==1, else comps[n-3] (n>=3) */
            uint64_t r_n = (current_n == 3) ? 1 : comps[current_n - 3];
            /* Difference (must be positive) */
            uint64_t diff = c_n - r_n;
            if (diff == 0) {
                ++current_n;
                continue;
            }

            uint64_t numerator = current_n / 4;   /* floor division */

            if (numerator % diff == 0) {
                uint64_t val = numerator / diff;
                /* Only keep non-prime results */
                if (!isPrimeGeneric((seq_t)val)) {
                    ++found;
                    if (found == n)
                        return (seq_t)val;
                }
            }
        }
        ++current_n;
    }
    /* unreachable */
    return (seq_t)0;
}
