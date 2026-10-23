/*
 * A163176  The n-th Minkowski number divided by the n-th factorial: a(n) =
 *          A053657(n)/n!.
 * https://oeis.org/A163176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A053657(n)/n! (the n‑th Minkowski number divided by n!)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for primes up to the largest n requested */
static size_t prime_count = 0;
static uint32_t *primes     = NULL;      /* array of primes <= max_primes */
static bool   *prime_mark  = NULL;      /* sieve mask */
static idx_t  max_primes   = 0;

/* Allocate / recompute the sieve up to |limit| */
static void
ensure_primes(idx_t limit)
{
    if (limit <= max_primes)
        return;          /* already ready */

    /* free old memory */
    free(prime_mark);
    free(primes);

    max_primes = limit;

    /* Allocate sieve array */
    prime_mark = (bool*)malloc(sizeof(bool) * (max_primes + 1));
    if (!prime_mark)
        return;          /* allocation failed – caller will see empty result */

    /* initialise all entries to true (prime) */
    for (idx_t i = 0; i <= max_primes; ++i)
        prime_mark[i] = true;
    if (max_primes >= 0) prime_mark[0] = false;
    if (max_primes >= 1) prime_mark[1] = false;

    for (idx_t i = 2; i * i <= max_primes; ++i) {
        if (!prime_mark[i])
            continue;
        for (idx_t j = i * i; j <= max_primes; j += i)
            prime_mark[j] = false;
    }

    /* count primes */
    prime_count = 0;
    for (idx_t i = 2; i <= max_primes; ++i)
        if (prime_mark[i])
            ++prime_count;

    /* store primes in an array */
    primes = (uint32_t*)malloc(sizeof(uint32_t) * prime_count);
    if (!primes)
        return;          /* allocation failed */

    size_t k = 0;
    for (idx_t i = 2; i <= max_primes; ++i)
        if (prime_mark[i])
            primes[k++] = (uint32_t)i;
}

/* fast exponentiation: base^exp   (exp is idx_t to keep loop limits small) */
static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1)
            res *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return res;
}

/* a(n) = A053657(n) / n! */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;                      /* undefined below offset – return 1 */

    ensure_primes(n);

    seq_t result = (seq_t)1;

    /* prime by prime */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = primes[idx];

        /* Minkowski exponent: sum_{k>=0} floor((n-1)/((p-1)p^k)) */
        idx_t exp_mink = 0;
        idx_t denom    = (idx_t)(p - 1);      /* (p-1)*p^k for current k */
        while (denom <= n - 1) {
            exp_mink += (n - 1) / denom;
            if (denom > (n - 1) / p)          /* stop before overflow of denom*=p */
                break;
            denom *= p;
        }

        /* factorial exponent: sum_{k>=1} floor(n/p^k) */
        idx_t exp_fact = 0;
        idx_t power    = p;
        while (power <= n) {
            exp_fact += n / power;
            if (power > n / p)                /* stop before overflow of power*=p */
                break;
            power *= p;
        }

        idx_t exponent = exp_mink - exp_fact;
        if (exponent > 0) {
            seq_t p_val = (seq_t)p;
            seq_t pow   = pow_seq(p_val, exponent);
            result *= pow;
        }
    }

    return result;
}
