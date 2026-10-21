/*
 * A229680  Number of defective 3-colorings of an n X 3 0..2 array connected
 *          diagonally and antidiagonally with exactly two mistakes, and
 *          colors introduced in row-major 0..2 order.
 * https://oeis.org/A229680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 0 * a(2) = 36 * a(n) = 2^n * 3^(n-3) * (-45 + 4*n +
 *            16*n^2) for n ≥ 3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 45,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;      /* offset 1 */
    if (n == 2) return (seq_t)36;     /* given value */

    /* 2^n and 3^(n-3) as seq_t */
    seq_t pow2n = pow2(n);
    seq_t pow3m = pow3(n - 3);

    /* polynomial part, computed with signed 64‑bit arithmetic */
    int64_t poly_int = -45 + 4 * (int64_t)n + 16 * (int64_t)n * (int64_t)n;
    seq_t poly = (seq_t)poly_int;

    /* multiply the three components */
    seq_t res = pow2n;
    res = res * pow3m;
    res = res * poly;
    return res;
}
