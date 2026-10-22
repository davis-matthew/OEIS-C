/*
 * A001309  Order of real Clifford group L_n connected with Barnes-Wall
 *          lattices in dimension 2^n.
 * https://oeis.org/A001309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(n^2+n+2) * (2^n-1) * Product_{k=1..n-1} (2^(2*k)-1),
 *            with a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   a001309_pow2, a001309_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a001309_pow2.h"
#include "../utils/a001309_prod.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    seq_t p = a001309_pow2(n * n + n + 2);
    if (p == 0) return p; /* fixed-width overflow: value is 0 mod 2^w */
    seq_t r = p * (a001309_pow2(n) - 1);
    r *= a001309_prod(1, n);
    return r;
}
