/*
 * A145444  Dirichlet g.f.: (1+3/4^s+2/8^s)*zeta(s)^3.
 * https://oeis.org/A145444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is the multiplicative function defined by * * a(2^e) =
 *            3*(e-1)*e + 3 (for e > 0) * a(p^e) = (e+1)*(e+2)/2 (for odd
 *            prime p) * a(1) = 1 * * Thus a(n) = Π_p^e ‑ prime power in n of
 *            the above local values
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    /* Factor n into prime powers and multiply the corresponding local
     * values.  We use unsigned 64‑bit integers for intermediate work,
     * casting to seq_t for the arithmetic with the sequence type.
     */
    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count;
    factor_u64(nn, primes, exps, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t val;
        if (p == 2) {
            seq_t ee = (seq_t)e;
            val = (seq_t)3 * (ee - 1) * ee + 3;
        } else {
            seq_t ee = (seq_t)e;
            val = (ee + 1) * (ee + 2) / 2;
        }
        result *= val;
    }
    return result;
}
