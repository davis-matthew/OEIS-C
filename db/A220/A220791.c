/*
 * A220791  Number of nonsingular n X n matrices over GF(13).
 * https://oeis.org/A220791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{i=0}^{n-1} (13ⁿ - 13ᶦ) (number of nonsingular n×n
 *            matrices over GF(13) )
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"   /* seq_pow(seq_t base, idx_t exp) */

/* Count nonsingular n x n matrices over GF(13). */
seq_t a(idx_t n)
{
    /* The empty product (n = 0) equals 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    const seq_t mod = (seq_t)13;        /* field size */
    /* Pre‑compute 13ⁿ as seq_t. */
    seq_t pn = seq_pow(mod, n);

    seq_t prod = (seq_t)1;   /* ∏(pn – 13ᶦ)  */
    seq_t powi = (seq_t)1;   /* keeps 13ᶦ */

    for (idx_t i = 0; i < n; ++i) {
        seq_t term = pn - powi;
        prod *= term;
        powi *= mod;          /* powi = 13ᶦ⁺¹ for next iteration */
    }

    return prod;
}
