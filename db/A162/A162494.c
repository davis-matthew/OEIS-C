/*
 * A162494  Number of reduced words of length n in the Weyl group E_8 on 8
 *          generators and order 696729600.
 * https://oeis.org/A162494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of
 *            (1‑x²)(1‑x⁸)(1‑x¹²)(1‑x¹⁴)(1‑x¹⁸)(1‑x²⁰)(1‑x²⁴)(1‑x³⁰)/(1‑x)⁸ in
 *            the generating function
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 56, u64 n <= 120, u128 n <= 120,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by inclusion‑exclusion: a(n) = Σ_S (-1)ⁿˢ C(n‑ΣS + 7, 7) where S ranges over subsets of {2,8,12,14,18,20,24,30} */

#include <stddef.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    const idx_t exps[8] = {(idx_t)2, (idx_t)8, (idx_t)12, (idx_t)14,
                            (idx_t)18, (idx_t)20, (idx_t)24, (idx_t)30};
    seq_t result = 0;
    for (int mask = 0; mask < 256; ++mask) {
        idx_t sum = 0;
        int parity = 0;
        for (int i = 0; i < 8; ++i) {
            if (mask & (1 << i)) {
                sum += exps[i];
                parity ^= 1; /* toggle parity */
            }
        }
        if (n < sum) continue;
        idx_t k = (n - sum) + (idx_t)7;
        seq_t term = binom(k, (idx_t)7);
        if (parity == 0) {
            result += term;
        } else {
            result -= term;
        }
    }
    return result;
}
