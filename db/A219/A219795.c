/*
 * A219795  Sum of the absolute values of the antidiagonals of the triangle
 *          A135929(n) companion. See the comment.
 * https://oeis.org/A219795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} abs(A192011(n-k,k)), a(0)=2.
 *            A192011 row r (r>=2) has entries only at even positions 2i, with
 *            |T(r,2i)| = |2*C(r-i,i) - 3*C(r-i-1,i-1)| (i=0 gives 2), so a(n)
 *            = Sum_{i=0..floor(n/4)} |T(n-2i,2i)|
 * Range:     u8 n <= 18, u16 n <= 35, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A219795_term(idx_t r, idx_t i)
{
    if (i == 0) return (seq_t)2;
    seq_t m = (seq_t)r - (seq_t)i;          /* r - i */
    seq_t c = 1;                            /* C(m-1, i-1) */
    for (idx_t j = 1; j + 1 <= i; j++) {
        c = c * (m - (seq_t)i + (seq_t)j) / (seq_t)j;
    }
    seq_t big = c * m / (seq_t)i;           /* C(m, i) */
    seq_t x = (seq_t)2 * big;
    seq_t y = (seq_t)3 * c;
    if (x >= y) return x - y;
    return y - x;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    seq_t s = 0;
    for (idx_t i = 0; i <= n / 4; i++) {
        s += A219795_term(n - 2 * i, i);
    }
    return s;
}
