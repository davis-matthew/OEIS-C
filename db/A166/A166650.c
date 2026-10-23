/*
 * A166650  Totally multiplicative sequence with a(p) = 10*(p+1) for prime p.
 * https://oeis.org/A166650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (10·(p+1))^e (multiplicative with a(p) =
 *            10·(p+1))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 ; a(0) (if called) returns 1 */

    uint64_t x = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(x, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)10 * ((seq_t)primes[i] + (seq_t)1);
        for (size_t j = 0; j < exps[i]; ++j)
            result *= term;
    }
    return result;
}
