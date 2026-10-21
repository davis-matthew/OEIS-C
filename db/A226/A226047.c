/*
 * A226047  Largest prime power dividing binomial(2n, n).
 * https://oeis.org/A226047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = max_{p prime ≤ 2n} p^{v_p(C(2n,n))} where v_p(x) is the
 *            exponent of prime p in x
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;             /* sequence offset is 1, undefined for 0 */

    idx_t limit = 2 * n;

    /* Sieve of Eratosthenes up to 'limit' */
    bool *is_prime = (bool *)malloc((size_t)(limit + 1));
    if (!is_prime) return (seq_t)0;           /* fallback on allocation failure */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; i <= limit / i; ++i)   /* i * i <= limit */
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;

    seq_t max_pow = 0;

    for (idx_t p = 2; p <= limit; ++p)
    {
        if (!is_prime[p]) continue;

        /* Legendre's formula for exponent of p in C(2n,n) */
        idx_t e = 0;
        idx_t pp = p;
        while (pp <= limit)
        {
            e += (limit / pp) - 2 * (n / pp);
            if (pp > limit / p) break;        /* avoid overflow when pp *= p */
            pp *= p;
        }

        if (e > 0)
        {
            seq_t pow = 1;
            for (idx_t i = 0; i < e; ++i) pow *= (seq_t)p;
            if (pow > max_pow) max_pow = pow;
        }
    }

    free(is_prime);
    return max_pow;
}
