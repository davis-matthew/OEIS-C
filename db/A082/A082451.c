/*
 * A082451  Sum over divisors d of n of Kronecker symbol (-60, d).
 * https://oeis.org/A082451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-60,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t contrib;
        if (p == 2 || p == 3 || p == 5) {
            contrib = 1;
        } else {
            uint64_t r = p % 15;
            if (r == 1 || r == 2 || r == 4 || r == 8) {
                contrib = (seq_t)(e + 1);
            } else if (r == 7 || r == 11 || r == 13 || r == 14) {
                contrib = ((e & 1) == 0) ? 1 : 0;
            } else {
                contrib = 1; /* should not occur */
            }
        }
        result *= contrib;
    }
    return result;
}
