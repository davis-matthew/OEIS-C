/*
 * A196524  a(n) = phi(n)*tau(n^2).
 * https://oeis.org/A196524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = phi(n) * tau(n^2) (tau is the divisor–count function)
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* sequence is defined for n≥1 */

    /* φ(n) */
    seq_t phi_n = phi(n);                  /* helper from utils/phi.h */

    /* factorise n: pr[] holds primes, ex[] holds exponents */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt); /* helper from utils/factor_u64.h */

    /* τ(n²) = ∏(2·e + 1) over all exponents e of n */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)ex[i];
        tau *= (seq_t)(2 * e + 1);
    }

    return phi_n * tau;
}
