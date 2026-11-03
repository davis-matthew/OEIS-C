/*
 * A228393  The number of permutations of length n that can be sorted by 3
 *          block transpositions.
 * https://oeis.org/A228393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{10} c_k · binom(n - k + 9, 9) where c = [1, -8,
 *            31, -66, 120, 3, 263, 17, 9, -1] (coef for x^k in P(x))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 27, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static const int coeff[11] = {
    /* index */ 0, /* unused */
    1, -8, 31, -66, 120, 3, 263, 17, 9, -1
};

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0;
    }
    seq_t result = 0;
    for (idx_t k = 1; k <= 10; ++k) {
        if (n < k) {
            break;
        }
        seq_t bin = binomial(n - k + 9, 9);
        int c = coeff[k];
        if (c > 0) {
            result += mul_small(bin, (unsigned)c);
        } else {
            result -= mul_small(bin, (unsigned)(-c));
        }
    }
    return result;
}
