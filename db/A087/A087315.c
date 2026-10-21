/*
 * A087315  a(n) = Product_{k=1..n} prime(k)^prime(n-k+1).
 * https://oeis.org/A087315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} prime(k)^{prime(n-k+1)} (OEIS A087315)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   seq_mul, seq_pow, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

extern seq_t seq_pow(seq_t base, idx_t exp);
extern seq_t seq_mul(seq_t a, seq_t b);

static uint32_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, ... */
static size_t primes_cnt = 0;        /* number of primes stored */

/* Ensure that the first n primes are available in the global array */
static void ensure_primes(idx_t n)
{
    if (n == 0)
        return;
    if (primes_cnt >= (size_t)n)
        return;

    /* Free any previously allocated array */
    if (primes != NULL)
        free(primes);

    sieve_first_n_primes(n, &primes, &primes_cnt);
}

/* Compute a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return (seq_t)1;

    ensure_primes(n);

    /* calculate the product ∏_{k=1..n} p_k^{p_{n-k+1}} */
    seq_t result = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* base = prime(k) = primes[k-1] */
        seq_t base = (seq_t)primes[k - 1];

        /* exponent = prime(n-k+1) = primes[n-k] */
        idx_t exp = (idx_t)primes[n - k];

        /* term = base^exp */
        seq_t term = seq_pow(base, exp);

        /* result *= term */
        result = seq_mul(result, term);
    }

    return result;
}
