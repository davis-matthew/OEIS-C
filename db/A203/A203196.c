/*
 * A203196  (n-1)-st elementary symmetric function of the first n terms of
 *          (2,1,4,3,6,5,8,7,...)=A103889.
 * https://oeis.org/A203196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e_{n-1} of the sequence (2,1,4,3,6,5,8,7,...).
 *            Recurrence: let t_k be the k-th element of the sequence (t_k =
 *            k+1 if k is odd, else k-1). Let P_k = Π_{i=1}^k t_i. Then a(1) =
 *            1, and for n ≥ 2 a(n) = P_{n-1} + t_n * a(n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t product = (seq_t)2;   /* P_1 */
    seq_t prev_a   = (seq_t)1;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        /* t_i = i+1 if i is odd, else i-1 */
        seq_t term = (i & 1) ? (seq_t)(i + 1) : (seq_t)(i - 1);
        seq_t current_a = product + term * prev_a;
        product = product * term;
        prev_a = current_a;
    }

    return prev_a;
}
