/*
 * A211791  a(n) = Sum_{y=1..n} Sum_{x=1..n} floor((x^k + y^k)^(1/k)) with k =
 *          2.
 * https://oeis.org/A211791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x=1..n} Sum_{y=1..n} floor(sqrt(x^2 + y^2))
 * Range:     u8 n <= 6, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root via Newton's method.
   Returns floor(sqrt(n)).  Works for 0 <= n <= UINT64_MAX. */
static inline idx_t integer_sqrt(idx_t n) {
    if (n < 2u) return n;
    idx_t x = n;
    idx_t y = (x + 1u) >> 1u;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1u;
    }
    return x;
}

seq_t a(idx_t n) {
    if (n == 0u) return 0;
    idx_t i, j;
    seq_t sum = 0;
    for (i = 1u; i <= n; ++i) {
        idx_t i2 = i * i;
        for (j = 1u; j <= n; ++j) {
            idx_t s = i2 + j * j;
            seq_t term = (seq_t)integer_sqrt(s);
            sum += term;
        }
    }
    return sum;
}
