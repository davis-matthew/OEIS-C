/*
 * A064146  Sum of non-unitary prime divisors of binomial(n,floor(n/2)).
 * https://oeis.org/A064146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3158, u16 n <= 7500, u32 n <= 7500, u64 n <= 7500,
 *            u128 n <= 7500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sum of all primes p such that the exponent of p in
 * the binomial coefficient C(n, floor(n/2)) is at least 2.
 * That is, for each prime p with vp(C(n, floor(n/2))) >= 2, add p to the sum.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Exponent of prime p in n! (Legendre's formula) */
static inline idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t res = 0;
    while (n)
    {
        n /= p;
        res += n;
    }
    return res;
}

/* --- Prime sieve, dynamic, global --- */
static idx_t   *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;      /* number of primes stored */
static idx_t     prime_lim  = 0;     /* largest number up to which primes are generated */

/* Ensure all primes up to 'limit' are generated and stored in prime_list. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_lim)
        return;                       /* already have enough */

    /* Allocate a temporary sieve array */
    unsigned char *is_composite = (unsigned char*)calloc((size_t)(limit + 1), 1);
    if (!is_composite)
        return;                       /* allocation failed – remain with old list */

    for (idx_t i = 2; i * i <= limit; ++i)
    {
        if (!is_composite[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            ++cnt;

    /* Reallocate prime_list if necessary */
    if (prime_cnt < cnt)
    {
        idx_t *tmp = (idx_t*)realloc(prime_list, cnt * sizeof(idx_t));
        if (tmp)
        {
            prime_list = tmp;
            prime_cnt  = cnt;
        }
        /* If realloc fails we keep the old data but the list may be incomplete */
    }

    /* Store primes */
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            prime_list[pos++] = i;

    prime_lim = limit;
    free(is_composite);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    idx_t k = n >> 1;          /* floor(n/2) */
    ensure_primes(n);          /* primes up to n are enough */

    seq_t sum = 0;

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = prime_list[i];
        if (p > n)
            break;

        idx_t e = vp_factorial(n, p)
                - vp_factorial(k, p)
                - vp_factorial(n - k, p);

        if (e >= 2)
            sum += (seq_t)p;
    }

    return sum;
}
