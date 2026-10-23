/*
 * A035754  Coordination sequence for C_17 lattice.
 * https://oeis.org/A035754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=0}^{17} binom(17,i) * binom(16+2n-i,16)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 25, u128 n <= 416,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Pre‑computed binomial coefficients C(17,i) */
static const seq_t binom17[18] = {
    1ULL, 17ULL, 136ULL, 680ULL, 2380ULL,
    6188ULL, 12376ULL, 19448ULL, 24310ULL, 24310ULL,
    19448ULL, 12376ULL, 6188ULL, 2380ULL, 680ULL,
    136ULL, 17ULL, 1ULL
};

/* Compute C(16+N,16) using the provided binomial routine */
static seq_t binom16_of_N(idx_t N)
{
    return binomial(16 + N, 16);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t two_n = n << 1;  /* 2*n */

    for (idx_t i = 0; i <= 17 && i <= two_n; ++i) {
        idx_t N = two_n - i;                /* N = 2n - i */
        seq_t coeff = binom16_of_N(N);       /* C(16+N,16) */
        result += binom17[i] * coeff;       /* C(17,i) * coeff */
    }
    return result;
}
