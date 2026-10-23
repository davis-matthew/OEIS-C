/*
 * A014685  In sequence of positive integers add 1 to first prime and subtract
 *          1 from 2nd prime; add 1 to 3rd prime and subtract 1 from 4th prime
 *          and so on.
 * https://oeis.org/A014685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: incremental sieve.  For n ≥ 1, a(n) = n if n is not prime,
   a(n) = n+1 if n is the k‑th prime with k odd, and a(n) = n-1 if k even. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static idx_t   max_computed = 0;        /* largest n processed so far   */
static idx_t *prime_index = NULL;      /* prime_index[n] = 0 if composite, else 1‑based index of prime */
static idx_t *primes      = NULL;      /* array of primes found          */
static size_t primes_used = 0;         /* number of primes stored       */
static size_t primes_cap  = 0;         /* capacity of primes array      */

/* Ensure that prime_index[0 .. n] is filled.  Uses trial division by known primes. */
static void ensure_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* Resize the prime_index array if needed. */
    prime_index = (idx_t*)realloc(prime_index, (n + 1) * sizeof(idx_t));
    if (!prime_index)
        exit(EXIT_FAILURE);

    /* Add new primes in the range (max_computed, n]. */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        int is_prime = 1;
        if (i < 2) {
            is_prime = 0;
        } else {
            for (size_t k = 0; k < primes_used; ++k) {
                idx_t p = primes[k];
                if (p > i / p)          /* p * p > i, avoid overflow */
                    break;
                if (i % p == 0) {
                    is_prime = 0;
                    break;
                }
            }
        }

        if (is_prime) {
            if (primes_used == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (idx_t*)realloc(primes, primes_cap * sizeof(idx_t));
                if (!primes)
                    exit(EXIT_FAILURE);
            }
            primes[primes_used++] = i;
            prime_index[i] = primes_used;       /* 1‑based index */
        } else {
            prime_index[i] = 0;
        }
    }

    max_computed = n;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    if (prime_index[n] == 0)
        return (seq_t)n;                       /* composite or 1 */
    else if (prime_index[n] & 1ULL)
        return (seq_t)n + (seq_t)1;             /* odd‑indexed prime */
    else
        return (seq_t)n - (seq_t)1;             /* even‑indexed prime */
}
