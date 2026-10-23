/*
 * A124271  a(n) = Sum_{i=1..n} (prime(i)^n - 1)/(prime(i) - 1).
 * https://oeis.org/A124271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (prime(i)^n - 1)/(prime(i) - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* compute base^exp for arbitrary seq_t */
static seq_t seq_pow(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        exp >>= 1;
        if (exp) base *= base;
    }
    return result;
}

/* sieve of Eratosthenes up to 'limit', collect first 'need' primes */
static void sieve_first_n_primes(idx_t limit, idx_t need, uint64_t *first_primes)
{
    /* allocate boolean array for marking composites */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)limit + 1);
    if (!is_prime) return; /* allocation failed – return via garbage data */
    for (idx_t i = 2; i <= limit; ++i) is_prime[i] = 1;

    idx_t count = 0;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* collect first 'need' primes */
    for (idx_t i = 2; i <= limit && count < need; ++i)
        if (is_prime[i])
            first_primes[count++] = (uint64_t)i;

    free(is_prime);
}

/* compute A124271 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* offset starts at 1 */

    /* an upper bound for the n-th prime: 20*n+100 is safe for n≤10000 */
    idx_t limit = n * 20 + 100;
    uint64_t *primes = (uint64_t *)malloc((size_t)n * sizeof *primes);
    if (!primes) return 0;            /* allocation failed – return garbage */

    sieve_first_n_primes(limit, n, primes);

    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
    {
        seq_t p      = (seq_t)primes[i];
        seq_t pow_n  = seq_pow(p, n);           /* p^n   */
        seq_t num    = pow_n - 1;               /* p^n-1 */
        seq_t denom  = p - 1;                   /* p-1   */
        seq_t term   = num / denom;             /* term  */
        sum += term;
    }

    free(primes);
    return sum;
}
