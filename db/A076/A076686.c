/*
 * A076686  a(n) = min(core(n),phi(n)) where core(n) is the squarefree part of
 *          n.
 * https://oeis.org/A076686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min( n / (largest square divisor of n) , φ(n) ) where
 *            core(n) is the squarefree part of n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/factor_uint64.h"   /* factor_uint64 */
#include "../utils/phi.h"            /* phi */

seq_t a(idx_t n)
{
    /* For n < 1 the definition is not requested – return 0. */
    if (n < 1)
        return 0;

    /* Factor n into primes and their exponents. */
    uint64_t primes[20];
    int      exps[20];
    int      cnt;
    factor_uint64((uint64_t)n, primes, exps, &cnt);

    /* Compute core(n) = n divided by the largest square dividing n.
       This equals the product of each prime raised to the parity
       (exponent modulo 2) of its exponent. */
    seq_t core = 1;
    for (int i = 0; i < cnt; ++i) {
        if (exps[i] & 1)          /* odd exponent → include the prime once */
            core *= (seq_t)primes[i];
    }

    /* Compute φ(n). */
    seq_t phi_val = phi(n);

    /* Return the smaller of core(n) and φ(n). */
    return core < phi_val ? core : phi_val;
}
