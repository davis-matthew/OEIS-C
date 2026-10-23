/*
 * A160897  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 8.
 * https://oeis.org/A160897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{d|n} Moebius(n/d)*d^(8-1)/phi(n) Multiplicative: for
 *            a prime power p^e, a(p^e) = p^(6e-6) * (p^7-1)/(p-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 35, u64 n <= 1319,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* factor n */
    uint64_t primes_arr[64];
    uint64_t exp_arr[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes_arr, exp_arr, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes_arr[i];
        idx_t    e  = (idx_t)exp_arr[i];

        /* compute p^(6e-6) = p^(6*(e-1)) */
        seq_t pow_p6;
        if (e == (idx_t)1)
            pow_p6 = (seq_t)1;
        else
            pow_p6 = seq_pow((seq_t)p, 6 * (e - (idx_t)1));

        /* compute S = 1 + p + p^2 + ... + p^6  */
        seq_t S      = (seq_t)1;
        seq_t pk     = (seq_t)p;          /* current p^k  */
        for (int k = 1; k <= 6; ++k) {
            S = S + pk;
            pk = seq_mul(pk, (seq_t)p);
        }

        /* term = p^(6e-6) * S */
        seq_t term = seq_mul(pow_p6, S);

        result = seq_mul(result, term);
    }

    return result;
}
