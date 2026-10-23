/*
 * A046073  Number of squares in multiplicative group modulo n.
 * https://oeis.org/A046073
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (p==2 ? 2^max(e-3,0) : (p-1)*p^(e-1)/2)
 * Range:     u8 n <= 292, u16 n <= 9831, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   factor_u64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"   /* factor_u64() */
#include "../utils/pow2.h"        /* pow2()      */
#include "../utils/pow_seq.h"     /* pow_seq()   */

seq_t a(idx_t n)
{
    /* A046073 is multiplicative.  We factor n and multiply the
       contribution of each prime power.  For n==1 the product of
       an empty set is 1, so no special case is required. */

    uint64_t nn = (uint64_t)n;

    /* storage for prime factors and their exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;

        if (p == 2)
        {
            /* a(2^e) = 2^max(e-3,0) */
            uint64_t exp = (e > 3) ? (e - 3) : 0;
            val = pow2((idx_t)exp);
        }
        else
        {
            /* a(p^e) = (p-1)*p^(e-1)/2 for odd primes */
            seq_t pow = pow_seq((seq_t)p, (int)(e - 1));
            val    = ((seq_t)(p - 1) * pow) / (seq_t)2;
        }

        result *= val;
    }

    return result;
}
