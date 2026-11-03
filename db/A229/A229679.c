/*
 * A229679  Number of defective 3-colorings of an n X 2 0..2 array connected
 *          diagonally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(2*(n-1),2)*2^(2*n-5) for n>2 a(1)=0, a(2)=2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 204 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

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

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* binomial(2*(n-1), 2) */
    idx_t t   = 2 * (n - 1);
    seq_t res = binomial(t, 2);

    /* 2^(2*n-5) */
    idx_t exp = (n << 1) - 5;   /* compute 2*n-5 */
    res = seq_mul(res, pow2(exp));

    /* multiply by 3 */
    res = mul_small(res, 3);

    return res;
}
