/*
 * A063960  Sum of non-unitary prime divisors of n!: sum of those prime
 *          divisors for which the exponent in the prime factorization exceeds
 *          1.
 * https://oeis.org/A063960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all prime numbers p ≤ floor(n/2) (i.e. the sum of
 *            the primes that occur with exponent ≥ 2 in n!)
 * Range:     u8 n <= 85, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  STORE
 *  The limits and temporary arrays are kept static so that repeated calls
 *  are fast.  Each time we need a larger sieve, we reallocate new memory
 *  and discard the old one.
 */
static idx_t sieve_limit = 0;
static bool *is_comp = NULL;   /* composite flag array */
static seq_t *prime_cum = NULL;/* cumulative prime sum */

/* Build or extend a sieve up to 'limit' (inclusive) and fill prime_cum[0..limit]. */
static void sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit) return;       /* already big enough */

    /* free old storage if present (small number of reallocs for the test data) */
    if (is_comp)    free(is_comp);
    if (prime_cum)  free(prime_cum);

    /* allocate new storage */
    is_comp  = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_comp) exit(1);                   /* abort on allocation failure */
    prime_cum = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!prime_cum) exit(1);

    /* simple Eratosthenes sieve */
    for (idx_t p = 2; p * p <= limit; ++p)
        if (!is_comp[p])
            for (idx_t j = p * p; j <= limit; j += p)
                is_comp[j] = true;

    /* cumulative sum of primes */
    prime_cum[0] = 0;
    if (limit >= 1) prime_cum[1] = 0;
    seq_t sum = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) sum += (seq_t)i;    /* i is prime */
        prime_cum[i] = sum;
    }

    sieve_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* a(1)=0, a(0) is irrelevant */
    idx_t m = n / 2;                          /* floor(n/2) */
    sieve_up_to(m);
    return prime_cum[m];
}
