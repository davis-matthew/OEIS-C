/*
 * A066449  a(n) = binomial(n, phi(n)), where phi(n) is the Euler totient
 *          function.
 * https://oeis.org/A066449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = binomial(n, phi(n))
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 67, u128 n <= 133,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 271 (2026-11-01)
 * Helpers:   binomial, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    /* Compute Euler's totient of n */
    seq_t phi_n = totient(n);
    /* φ(n) is guaranteed ≤ n, so it fits in idx_t */
    idx_t k = (idx_t)phi_n;
    /* Return the binomial coefficient C(n, φ(n)) */
    return binomial(n, k);
}
