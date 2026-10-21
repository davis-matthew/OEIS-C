/*
 * A056056  Square root of largest square dividing n-th central binomial
 *          coefficient.
 * https://oeis.org/A056056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 125, u16 n <= 362, u32 n <= 1594, u64 n <= 6240,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = sqrt( largest square dividing binomial(n, floor(n/2)) )
 * This uses Legendre's formula to compute the exponent of each prime in
 * binomial(n, floor(n/2)) and takes the integer square root of the
 * resulting product.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* fast exponentiation in seq_t, exponent is small (fits in idx_t) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t ans = 1;
    idx_t k = n >> 1;           /* floor(n/2) */
    idx_t nmk = n - k;          /* n - k  */

    uint64_t *primes;
    size_t prime_cnt;
    /* generate all primes ≤ n */
    build_primes_upto(n, &primes, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = primes[i];

        /* v_p(n!) , v_p(k!) , v_p((n-k)!) using Legendre's formula */
        idx_t exp_n = 0, exp_k = 0, exp_nmk = 0;
        idx_t temp = n;
        while (temp)
        {
            temp /= p;
            exp_n += temp;
        }
        temp = k;
        while (temp)
        {
            temp /= p;
            exp_k += temp;
        }
        temp = nmk;
        while (temp)
        {
            temp /= p;
            exp_nmk += temp;
        }

        idx_t e = exp_n - exp_k - exp_nmk;
        if (e == 0) continue;
        idx_t half = e >> 1;             /* floor(e/2) */
        if (half == 0) continue;

        ans *= pow_seq((seq_t)p, half);
    }

    free(primes);
    return ans;
}
