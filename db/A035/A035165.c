/*
 * A035165  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -25.
 * https://oeis.org/A035165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∑_{d|n} Kronecker(-25,d). The sequence is multiplicative
 *            with a(5^e)=1, a(2^e)=e+1, a(p^e)=e+1 for p≡1(mod4) (p≠5), and
 *            a(p^e)=1 if e is even, 0 if e is odd, for p≡3(mod4)
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
    /* For the OEIS offset a(1)=1.  n≥1 in the spec.  Return 0 for n==0 just
       to keep the function total, though offset 0 is undefined. */
    if (n == 0U)
        return 0;

    /* Factor n.  The utility gives prime factors in `primes` and their
       exponents in `exps`, with count `cnt`. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0U;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0U; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t factor;

        if (p == 5U) {
            factor = (seq_t)1;
        } else if (p == 2U || (p & 3U) == 1U) {
            /* p==2 or p%4==1 */
            factor = (seq_t)(e + 1U);
        } else { /* p%4==3 (includes 3) */
            factor = ((e & 1U) == 0U) ? (seq_t)1 : (seq_t)0;
        }
        result *= factor;
    }
    return result;
}
