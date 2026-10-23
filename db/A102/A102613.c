/*
 * A102613  Numerators of the reduced fractions of the ratios of the number of
 *          primes less than n over the number of composites less than n.
 * https://oeis.org/A102613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = numerator of pi(n)/(n - pi(n)) Equivalent to a(n) = pi(n)
 *            / gcd(pi(n), n)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_seq_t.h"

/* Static state for the sieve and prefix counts */
static bool   *sieve     = NULL;
static seq_t  *pi_arr    = NULL;
static idx_t   sieve_sz  = 0;

/* Extend the sieve and pi array to cover all numbers up to N (inclusive) */
static void extend_sieve(idx_t N)
{
    idx_t new_sz = N + 1;

    /* Reallocate the sieve array */
    bool *new_sieve = (bool*)realloc(sieve, new_sz * sizeof(bool));
    if (!new_sieve) exit(EXIT_FAILURE);
    sieve = new_sieve;

    /* Initialise all entries as prime, then fix 0 and 1 */
    for (idx_t i = 0; i < new_sz; ++i) sieve[i] = true;
    if (new_sz > 0) sieve[0] = false;
    if (new_sz > 1) sieve[1] = false;

    /* Basic Eratosthenes sieve */
    for (idx_t p = 2; p * p <= N; ++p)
        if (sieve[p])
            for (idx_t m = p * p; m <= N; m += p)
                sieve[m] = false;

    /* Compute prefix prime counts */
    seq_t *new_pi = (seq_t*)malloc(new_sz * sizeof(seq_t));
    if (!new_pi) exit(EXIT_FAILURE);
    seq_t count = 0;
    for (idx_t i = 0; i < new_sz; ++i) {
        if (i >= 2 && sieve[i]) ++count;
        new_pi[i] = count;
    }

    /* Replace the old table */
    free(pi_arr);
    pi_arr = new_pi;
    sieve_sz = new_sz;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* outside the defined offset */

    if (n >= sieve_sz)
        extend_sieve(n);

    seq_t pi_n  = pi_arr[n];          /* π(n) */
    seq_t n_seq = n;                  /* cast n to seq_t for GCD */

    seq_t d          = gcd_seq_t(pi_n, n_seq);
    seq_t numerator  = pi_n / d;      /* reduced numerator */

    return numerator;
}
