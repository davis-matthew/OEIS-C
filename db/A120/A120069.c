/*
 * A120069  Denominators of partial sums of a convergent series involving
 *          scaled Catalan numbers A000108.
 * https://oeis.org/A120069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator(Catalan(n)/4^(n-1))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator is a power of two, given by 2^(exp) where */
/* exp = max(0, 2*n - 2 - v2(Catalan(n))) */
/* v2(Catalan(n)) = 2*popcount(n) - popcount(2*n) - v2(n+1) */

#include "../utils/pow2.h"
#include <stdint.h>

static inline idx_t popcount_u64(idx_t x) {
    return (idx_t)__builtin_popcountll((unsigned long long)x);
}
static inline idx_t v2_u64(idx_t x) {
    return (idx_t)__builtin_ctzll((unsigned long long)x);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1; unused for n==0 */
    idx_t pc_n  = popcount_u64(n);
    idx_t pc_2n = popcount_u64(n << 1);   /* 2*n fits in 64 bits for the problem bounds */
    idx_t v2_n1 = v2_u64(n + 1);

    /* v2(Catalan(n)) = 2*popcount(n) - popcount(2n) - v2(n+1) */
    idx_t v2_C = (idx_t)(2 * pc_n - pc_2n - v2_n1);

    idx_t exp = (idx_t)(2 * n - 2);
    if (exp < v2_C) exp = 0;
    else exp -= v2_C;

    return pow2(exp);
}
