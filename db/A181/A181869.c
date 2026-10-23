/*
 * A181869  a(1) = 2, a(2) = 1. For n >= 3, a(n) is found by concatenating the
 *          squares of the first n-1 terms of the sequence in reverse order
 *          and then dividing the resulting number by a(n-1).
 * https://oeis.org/A181869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(1) = 2, a(2) = 1; for n >= 3, a(n) is found by a(3) =
 *            concatenate(a(2)^2,a(1)^2)/a(2) and for n >= 4, a(n) =
 *            10^Pell(n-2) * a(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)14;  /*  concat(1²,2²)/1 = 14 */

    /* a(2) and a(3) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)14;  /* a(n-1) */
    seq_t a_cur = a_nm1;

    /* Pell numbers: P(1)=1, P(2)=2 */
    idx_t pell_prev = 1;  /* P(n-1) for current k */
    idx_t pell_cur  = 2;  /* P(n)   for current k */

    for (idx_t i = 4; i <= n; ++i) {
        /* Use pell_cur as Pell(i-2) */
        seq_t pow10p = pow10(pell_cur);
        a_cur = seq_mul(pow10p, a_nm1) + a_nm2;

        /* Shift Pell values for next i */
        idx_t pell_next = 2 * pell_cur + pell_prev;
        pell_prev = pell_cur;
        pell_cur  = pell_next;

        /* Shift sequence values */
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_cur;
}
