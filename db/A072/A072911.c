/*
 * A072911  Number of "phi-divisors" of n.
 * https://oeis.org/A072911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π φ(r_i) where n = Π p_i^{r_i}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_uint64, phi_of_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the factor_uint64 helper from utils (provides prime factors and exponents). */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_uint64.h"
#include "../utils/phi_of_small.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* behaviour for n < 1 is irrelevant */

    uint64_t fac[20];             /* prime bases of n */
    int      exp[20];             /* corresponding exponents */
    int      cnt;                 /* number of distinct primes */

    factor_uint64((uint64_t)n, fac, exp, &cnt);

    seq_t product = 1;
    for (int i = 0; i < cnt; ++i) {
        seq_t phi_exp = phi_of_small(exp[i]);
        product *= phi_exp;
    }

    return product;
}
