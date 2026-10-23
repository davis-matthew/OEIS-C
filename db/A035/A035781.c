/*
 * A035781  Coordination sequence for C_44 lattice.
 * https://oeis.org/A035781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^(2n) in ((1+x)/(1-x))^44
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 31,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    idx_t twoN = n << 1;
    idx_t limit = twoN < 44 ? twoN : 44;               /* i up to min(44, 2n) */

    /* Pre‑compute 44 choose i once */
    static idx_t comb44[45];
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        comb44[0] = 1;
        for (idx_t i = 1; i <= 44; ++i) {
            /* comb44[i] = 44C(i) */
            comb44[i] = comb44[i - 1] * (44 - i + 1) / i;
        }
    }

    seq_t sum = 0;
    for (idx_t i = 0; i <= limit; ++i) {
        seq_t term = (seq_t)comb44[i] * binomial(43 + twoN - i, 43);
        sum += term;
    }
    return sum;
}
