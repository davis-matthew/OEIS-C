/*
 * A197130  Sum of reflection (or absolute) lengths of all elements in the
 *          Coxeter group of type B_n.
 * https://oeis.org/A197130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n*a(n-1) + 2^(n-1)*(n-1)!*(2n-1) with a(1)=1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t a_prev = 1;
    if (n == 1) return a_prev;

    seq_t B = 1;  /* B_k = 2^(k-1) * (k-1)! */
    for (idx_t k = 2; k <= n; ++k) {
        B = B * (seq_t)(2 * (k - 1));           /* update to B_k */
        seq_t term2 = B * (seq_t)(2 * k - 1);    /* B_k * (2k-1) */
        a_prev = a_prev * (seq_t)(2 * k) + term2;
    }
    return a_prev;
}
