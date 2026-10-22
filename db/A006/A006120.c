/*
 * A006120  Sum of Gaussian binomial coefficients [ n,k ] for q=6.
 * https://oeis.org/A006120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (6^(n-1)-1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev = 1;   /* a(i-2) */
    seq_t cur = 2;    /* a(i-1) */
    seq_t p = 6;      /* 6^(i-1) */
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t next = 2 * cur + (p - 1) * prev;
        prev = cur;
        cur = next;
        p *= 6;
    }
    return cur;
}
