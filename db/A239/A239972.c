/*
 * A239972  a(n) is the smallest m greater than n such that both the numerator
 *          and the denominator of the abundancy index of m are greater than
 *          those of n.
 * https://oeis.org/A239972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest m>n whose reduced abundancy index has both
 *            numerator > numerator of a(n) and denominator > denominator of
 *            a(n). The abundancy index of x is sigma(x)/x reduced to lowest
 *            terms
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

/* Return sigma(n) computed by factorisation using the precomputed primes. */
static seq_t sigma_uint64(uint64_t n)
{
    seq_t result = 1;
    uint64_t temp = n;

    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= temp; ++i)
    {
        uint64_t p = prime_list[i];
        if (temp % p == 0)
        {
            seq_t term = 1;
            seq_t pow  = 1;
            while (temp % p == 0)
            {
                temp /= p;
                pow *= p;
                term += pow;
            }
            result *= term;
        }
    }

    if (temp > 1)
        result *= (1 + temp);

    return result;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return 0;  /* undefined for n<1, but safe fallback */

    if (!prime_list)
        build_primes_upto((idx_t)100000, &prime_list, &prime_cnt);

    seq_t n_seq   = (seq_t)n;
    seq_t sigma_n = sigma_uint64(n);
    seq_t g       = gcd(sigma_n, n_seq);
    seq_t num_n   = sigma_n / g;
    seq_t den_n   = n_seq / g;

    for (idx_t m = n + 1; ; ++m)
    {
        seq_t m_seq   = (seq_t)m;
        seq_t sigma_m = sigma_uint64(m);
        seq_t g2      = gcd(sigma_m, m_seq);
        seq_t num_m   = sigma_m / g2;
        seq_t den_m   = m_seq / g2;

        if (num_m > num_n && den_m > den_n)
            return (seq_t)m;
    }
}
