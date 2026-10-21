/*
 * A055774  Least common multiple of n! and n^n.
 * https://oeis.org/A055774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n!, n^n) = ∏_{p≤n} p^{max(v_p(n!), n·v_p(n))}
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   where v_p(x) is the exponent of the prime p in the factorisation of x. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

/* Incrementally builds a list of all primes ≤ limit.
   The static data are kept between calls so that future calls with a larger
   limit need only extend the sieve. */
static void sieve_primes(idx_t limit, int **prime_list, size_t *prime_cnt)
{
    static idx_t current_limit = 0;
    static int *primes = NULL;
    static size_t primes_cnt = 0;

    if (limit <= current_limit) {
        *prime_list = primes;
        *prime_cnt  = primes_cnt;
        return;
    }

    /* Allocate a fresh boolean sieve array */
    bool *is_prime = (bool*)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) {
        *prime_list = NULL;
        *prime_cnt  = 0;
        return;
    }

    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t k = p * p; k <= limit; k += p)
                is_prime[k] = false;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    int *new_primes = (int*)malloc(cnt * sizeof(int));
    if (!new_primes) {
        free(is_prime);
        *prime_list = NULL;
        *prime_cnt  = 0;
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) new_primes[idx++] = (int)i;

    free(is_prime);

    /* Replace old static data with new data */
    free(primes);
    primes = new_primes;
    primes_cnt = cnt;
    current_limit = limit;

    *prime_list = primes;
    *prime_cnt  = primes_cnt;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n=0 under offset 1 */

    int *primes = NULL;
    size_t prime_cnt = 0;
    sieve_primes(n, &primes, &prime_cnt);
    if (!primes || prime_cnt == 0)
        return (seq_t)1;               /* safe fallback */

    seq_t result = 1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        int p = primes[i];

        /* exponent of p in n!   : Legendre's formula */
        idx_t e_fact = 0;
        idx_t pk = p;
        while (pk <= n) {
            e_fact += n / pk;
            if (pk > n / p)             /* prevent overflow of pk * p */
                break;
            pk *= p;
        }

        /* exponent of p in n   : multiplicity in n */
        idx_t e_n = 0;
        idx_t tmp = n;
        while (tmp % p == 0) {
            ++e_n;
            tmp /= p;
        }

        /* exponent of p in n^n : n * e_n */
        idx_t e_pow = e_n * n;

        idx_t e_max = e_fact > e_pow ? e_fact : e_pow;
        if (e_max > 0) {
            seq_t mult = pow_seq((seq_t)p, e_max);
            result *= mult;
        }
    }

    return result;
}
