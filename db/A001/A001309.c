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
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

/* Product_{k=lo..hi-1} (2^(2k)-1), via a balanced product tree */
static seq_t a001309_prod(idx_t lo, idx_t hi) {
    if (hi <= lo) return (seq_t)1;
    if (hi - lo == 1) return pow2(2 * lo) - 1;
    idx_t mid = lo + (hi - lo) / 2;
    seq_t left = a001309_prod(lo, mid);
    seq_t right = a001309_prod(mid, hi);
    return left * right;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    seq_t p = pow2(n * n + n + 2);
    if (p == 0) return p; /* fixed-width overflow: value is 0 mod 2^w */
    seq_t r = p * (pow2(n) - 1);
    r *= a001309_prod(1, n);
    return r;
}
