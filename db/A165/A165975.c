/*
 * A165975  a(n) = sqrt( binomial(4n,0) * binomial(4n,1) * ... *
 *          binomial(4n,2n-1) ).
 * https://oeis.org/A165975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt( binomial(4n,0) * binomial(4n,1) * ... *
 *            binomial(4n,2n-1) )
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow, seq_mul, seq_one, sumVpfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"
#include "../utils/sumVpfact.h"

/* Legendre formula: exponent of prime p in m!  */
static uint64_t vp_factorial(uint64_t m, uint32_t p)
{
    uint64_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* Simple sieve generating prime numbers up to limit (inclusive) */
static void build_primes(uint32_t limit, uint32_t **primes_out, size_t *cnt_out)
{
    bool *is_comp = (bool*)malloc((size_t)(limit + 1));
    memset(is_comp, 0, (size_t)(limit + 1));

    size_t cnt = 0;
    uint32_t *pr = (uint32_t*)malloc((size_t)(limit + 1) * sizeof(uint32_t));

    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            pr[cnt++] = i;
            if (i <= limit / i) {
                for (uint32_t j = i * i; j <= limit; j += i)
                    is_comp[j] = true;
            }
        }
    }
    free(is_comp);
    *primes_out = pr;
    *cnt_out = cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return seq_one();               /* a(0) = 1 */

    const uint64_t N   = (uint64_t)n;
    const uint64_t M4  = 4 * N;                 /* 4n  */
    const uint64_t M2  = 2 * N;                 /* 2n  */

    /* Generate primes up to 4n */
    uint32_t *primes;
    size_t prime_cnt;
    build_primes((uint32_t)M4, &primes, &prime_cnt);

    seq_t result = seq_one();

    for (size_t idx = 0; idx < prime_cnt; ++idx) {
        uint32_t p = primes[idx];

        uint64_t vp4 = vp_factorial(M4, p);          /* v_p((4n)!)   */
        uint64_t vp2 = vp_factorial(M2, p);          /* v_p((2n)!)   */

        uint64_t sum4 = sumVpfact(M4, p);            /* Σ_{k=1}^{4n} v_p(k!) */

        /* exponent in product */
        uint64_t e = (uint64_t)(2 * N) * vp4 - sum4 + vp2;
        uint64_t exp_half = e / 2;                   /* since product is a perfect square */

        if (exp_half != 0) {
            seq_t part = big_pow((seq_t)p, (seq_t)exp_half);
            result = seq_mul(result, part);
        }
    }

    free(primes);
    return result;
}
