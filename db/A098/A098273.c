/*
 * A098273  Array by antidiagonals: Number of planar lattice walks of length
 *          3n+2k starting at (0,0) and ending at (k,0), remaining in the
 *          first quadrant and using only NE,W,S steps.
 * https://oeis.org/A098273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   a(n) = T(r,k) where (r,k) are obtained by reading the * triangle
 *            of T(n,k) along antidiagonals. T(n,k) = * 4^n * (2k+1) /
 *            [(n+k+1)(2n+2k+1)] * C(2k,k) * C(3n+2k,n)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 41, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   index_to_rc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compile with -std=c11 or -std=gnu++17 in the build environment.  */
#include <stdbool.h>
#include "../utils/index_to_rc.h"

static inline seq_t pow_seq_t(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Compute binomial coefficient C(n,k) via multiplicative formula
 * (works for all idx_t values).  The intermediate result stays an
 * integer because each step divides the product exactly. */
static inline seq_t binom_n_k(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* Triangle entry T(n,k) according to the formula above. */
static inline seq_t T_entry(idx_t n, idx_t k) {
    seq_t pow4 = pow_seq_t(4, n);
    seq_t binom2k = binom_n_k(2 * k, k);
    seq_t binom3  = binom_n_k(3 * n + 2 * k, n);

    seq_t res = pow4;
    res *= binom2k;
    res *= binom3;
    res *= (seq_t)(2 * k + 1);

    res /= (seq_t)(n + k + 1);
    res /= (seq_t)(2 * n + 2 * k + 1);
    return res;
}

/* --- API function required by the build environment --- */
seq_t a(idx_t n) {
    idx_t r, k;
    index_to_rc(n, &r, &k);
    return T_entry(r, k);
}
