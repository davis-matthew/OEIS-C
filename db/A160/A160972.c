/*
 * A160972  a(n) = Sum_{d|n} Moebius(n/d)*d^(b-1)/phi(n) for b = 13.
 * https://oeis.org/A160972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = J_12(n)/phi(n) (multiplicative with a(p^e) = p^(11e-11) *
 *            (p^12 - 1)/(p-1) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 53, u128 n <= 2819,
 *            big any n
 * Helpers:   factor_u64, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 */
    uint64_t num = (uint64_t)n;

    /* factorization tables: large enough for n up to 10^4 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    if (num > 1)
        factor_u64(num, primes, exps, &cnt);
    /* For n==1, cnt will stay 0 and result below stays 1 */

    seq_t result = seq_one();           /* multiplicative identity */

    for (size_t j = 0; j < cnt; ++j) {
        uint64_t p   = primes[j];
        uint64_t e   = exps[j];

        /* term for prime power p^e:
           sum_{i=0}^{11} p^(i + 11*(e-1)) */
        seq_t term = 0;
        idx_t base_offset = 11 * ((idx_t)e - 1);

        for (idx_t i = 0; i < 12; ++i) {
            seq_t val = seq_pow((seq_t)p, base_offset + i);
            term += val;
        }

        result *= term;  /* multiplicative */
    }

    return result;
}
