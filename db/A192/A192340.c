/*
 * A192340  Constant term of the reduction of n-th polynomial at A158985 by
 *          x^2->x+1.
 * https://oeis.org/A192340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192340
 *
 * Let (a_k, b_k) be the reduction of the k‑th polynomial p_k(x)
 * from A158985 modulo x² − x − 1, so that
 *   p_k(x) ≡ a_k + b_k x   (mod x² − x − 1).
 *
 * The polynomials satisfy   p_{k+1}(x) = p_k(x)² + 1.
 * Consequently the reduced form satisfies the simple recurrences
 *   a_{k+1} = a_k² + b_k² + 1
 *   b_{k+1} = 2 a_k b_k + b_k²
 * with initial values a_0 = 1, b_0 = 1.
 *
 * The required sequence is
 *   a(1) = a_0 = 1,
 *   a(n) = a_{n-1}  for n ≥ 2.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a_0 = 1, b_0 = 1  */
    seq_t a_curr = 1, b_curr = 1;

    if (n <= 1)          /* offset starts at 1 */
        return a_curr;

    for (idx_t i = 1; i < n; ++i) {   /* compute up to a_{n-1} */
        seq_t a_next = a_curr * a_curr + b_curr * b_curr + (seq_t)1;
        seq_t b_next = (seq_t)2 * a_curr * b_curr + b_curr * b_curr;
        a_curr = a_next;
        b_curr = b_next;
    }
    return a_curr;
}
