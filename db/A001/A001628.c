/*
 * A001628  Convolved Fibonacci numbers.
 * https://oeis.org/A001628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/(1-x-x^2)^3, computed via c_k(n) = c_k(n-1) + c_k(n-2) +
 *            c_{k-1}(n), with c_0(n) = [n==0], c_k(-1) = c_k(-2) = 0, and
 *            a(n) = c_3(n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 34, u64 n <= 77, u128 n <= 166,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t p1a = 0, p1b = 0; /* c_1(i-1), c_1(i-2) */
    seq_t p2a = 0, p2b = 0; /* c_2(i-1), c_2(i-2) */
    seq_t p3a = 0, p3b = 0; /* c_3(i-1), c_3(i-2) */
    idx_t i;
    for (i = 0; i <= n; i++) {
        seq_t c0 = (i == 0) ? (seq_t)1 : (seq_t)0;
        seq_t c1 = p1a + p1b + c0;
        seq_t c2 = p2a + p2b + c1;
        seq_t c3 = p3a + p3b + c2;
        p1b = p1a; p1a = c1;
        p2b = p2a; p2a = c2;
        p3b = p3a; p3a = c3;
    }
    return p3a;
}
