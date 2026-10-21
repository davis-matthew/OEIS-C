/*
 * A095218  (Product of composite numbers among next n numbers) - (product of
 *          primes among next n numbers).
 * https://oeis.org/A095218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (product of composite numbers in [n(n-1)/2+1 , n(n+1)/2])
 *            - (product of primes in the same interval)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Global sieve data */
static idx_t sieve_limit = 0;
static uint8_t *is_prime_arr = NULL;

/* Extend the primality sieve to at least 'need'. Rebuilds the sieve from scratch. */
static void extend_sieve(idx_t need)
{
    if (need <= sieve_limit)
        return;
    uint8_t *new_arr = (uint8_t *)calloc((size_t)(need + 1), sizeof(uint8_t));
    if (!new_arr)
        return; /* allocation failed: leave array empty, will recompute next time */
    /* mark all indices >= 2 as primes initially */
    for (idx_t i = 2; i <= need; ++i)
        new_arr[i] = 1;
    /* Eratosthenes: mark composites */
    for (idx_t p = 2; p * p <= need; ++p) {
        if (new_arr[p]) {
            for (idx_t m = p * p; m <= need; m += p)
                new_arr[m] = 0;
        }
    }
    /* Clean up old array */
    if (is_prime_arr)
        free(is_prime_arr);
    is_prime_arr = new_arr;
    sieve_limit = need;
}

/* Computes a(n) for the described sequence */
seq_t a(idx_t n)
{
    /* Calculate bounds using seq_t for safety */
    seq_t nn = (seq_t)n;
    seq_t U_seq = nn * (nn + 1) / 2;
    seq_t L_seq = nn * (nn - 1) / 2 + 1;
    idx_t U = (idx_t)U_seq;
    idx_t L = (idx_t)L_seq;

    /* Ensure primes up to U are available */
    extend_sieve(U);

    seq_t composite_prod = 1;
    seq_t prime_prod     = 1;

    for (idx_t x = L; x <= U; ++x) {
        seq_t val = (seq_t)x;
        if (is_prime_arr[x])
            prime_prod *= val;
        else
            composite_prod *= val;
    }

    return composite_prod - prime_prod;
}
