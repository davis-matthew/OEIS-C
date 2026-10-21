/*
 * A097932  Positive integers n such that 2n-19 is prime.
 * https://oeis.org/A097932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (p_n + 19)/2 where p_n is the n‑th prime > 2
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Store all primes ≤ the last used sieve limit.  */
static idx_t *primes = NULL;          /* array of primes, primes[0] = 2 */
static size_t prime_count = 0;        /* number of primes stored */
static idx_t sieve_limit = 0;         /* last sieve limit used */

/* Generate all primes ≤ limit, storing them in 'primes'. */
static void generate_primes(idx_t limit)
{
    /* Allocate sieve array: 0 = prime, 1 = composite */
    char *is_comp = (char *)calloc((size_t)(limit + 1), 1);
    if (!is_comp) return;            /* allocation failure */

    is_comp[0] = 1;
    if (limit >= 1) is_comp[1] = 1;

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_comp[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) ++cnt;

    /* Allocate and fill primes array */
    idx_t *new_primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!new_primes)
    {
        free(is_comp);
        return;                       /* allocation failure */
    }

    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_comp[i]) new_primes[k++] = i;

    /* Replace old data */
    free(primes);
    primes = new_primes;
    prime_count = cnt;
    sieve_limit = limit;

    free(is_comp);
}

/* Ensure that primes[n] exists.  */
static void ensure_primes(idx_t n)
{
    if (prime_count > (size_t)n) return;    /* already enough */

    /* Start with a modest limit; double until sufficient. */
    idx_t limit = sieve_limit ? sieve_limit : 200000;
    while (prime_count <= (size_t)n)
    {
        limit <<= 1;                        /* multiply by 2 */
        generate_primes(limit);
        if (!primes) return;                /* allocation failure */
    }
}

/* Return the n‑th term of A097932.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n < 1, but safe */

    ensure_primes(n);

    /* primes[0] = 2, primes[1] = 3, so primes[n] is the prime > 2. */
    seq_t val = ((seq_t)primes[(size_t)n] + (seq_t)19) / 2;
    return val;
}
