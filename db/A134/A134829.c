/*
 * A134829  Denominator of moments of Chebyshev U- (or S-) polynomials.
 * https://oeis.org/A134829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = 1 if n is odd; otherwise a(n) = 2^(n – v₂(Catalan(n/2))),
 *            where v₂ denotes the exponent of 2 in an integer and Catalan(k)
 *            = binom(2k, k)/(k+1) is the k‑th Catalan number
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"

/* Count exponent of 2 in m! with m a 64‑bit integer */
static idx_t v2_factorial(idx_t m) {
    idx_t exp = 0;
    while (m > 1) {
        m >>= 1;                /* m = ⌊m/2⌋ */
        exp += m;
    }
    return exp;
}

/* Count trailing zeros (exponent of 2) in a 64‑bit unsigned integer */
static idx_t v2_uint64(uint64_t x) {
    idx_t exp = 0;
    while ((x & 1ULL) == 0ULL) {
        x >>= 1;
        ++exp;
    }
    return exp;
}

/* Return the denominator of the moment r(n) of Chebyshev U‑polynomials. */
seq_t a(idx_t n) {
    /* Odd n → denominator is 1 */
    if ((n & 1ULL) != 0) {
        return (seq_t)1;
    }

    /* Even n = 2k */
    idx_t k = n >> 1;

    /* v₂(Catalan(k)) = v₂((2k)!) – 2·v₂(k!) – v₂(k+1) */
    idx_t v2_2kfact   = v2_factorial(2 * k);
    idx_t v2_kfact    = v2_factorial(k);
    idx_t v2_kplus1   = v2_uint64((uint64_t)(k + 1));
    idx_t v2_catalan  = v2_2kfact - 2 * v2_kfact - v2_kplus1;

    /* Denominator exponent = n – v₂(Catalan(k)) = 2k – v₂(Catalan(k)) */
    idx_t denom_exp = n - v2_catalan;

    /* a(n) = 2^denom_exp */
    return pow2(denom_exp);
}
