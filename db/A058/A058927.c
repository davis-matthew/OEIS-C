/*
 * A058927  Numerators of series related to triangular cacti.
 * https://oeis.org/A058927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator(((2*n+1)^(n-1))/(2^n * n!))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the numerator of the reduced fraction ((2n+1)^(n-1))/(2^n*n!) */

#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow_seq.h"

static size_t primes_limit = 0;
static idx_t *primes = NULL;
static size_t primes_cnt = 0;

/* Prime exponent in n! (Legendre's formula) */
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

/* Ensure we have all primes up to limit.  The utility allocates the array. */
static void ensure_primes(idx_t limit)
{
    if (primes_limit < limit)
    {
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) is defined as 1 */

    idx_t limit = 2 * n + 1;      /* 2n+1 */
    ensure_primes(limit);

    seq_t result = (seq_t)1;
    idx_t temp = limit;           /* copy of 2n+1 for factoring */

    for (size_t i = 0; i < primes_cnt && primes[i] <= limit; ++i)
    {
        idx_t p = primes[i];

        /* exponent of p in (2n+1) */
        idx_t cnt = 0;
        while (temp % p == 0)
        {
            ++cnt;
            temp /= p;
        }

        idx_t e_num = (n > 0) ? (idx_t)(n - 1) * cnt : 0;

        /* exponent of p in 2^n * n! */
        idx_t denom_exp = 0;
        if (p == 2)
            denom_exp += n;
        denom_exp += vp_factorial(n, p);

        idx_t e_res = e_num > denom_exp ? e_num - denom_exp : 0;

        if (e_res > 0)
        {
            seq_t powp = pow_seq((seq_t)p, (int)e_res);
            result = result * powp;
        }
    }
    return result;
}
