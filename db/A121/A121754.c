/*
 * A121754  Number of columns ending at an even level in all deco polyominoes
 *          of height n. A deco polyomino is a directed column-convex
 *          polyomino in which the height, measured along the diagonal, is
 *          attained only in the last column.
 * https://oeis.org/A121754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) =
 *            (2n-3)a(n-1)-(n-1)(n-3)a(n-2)+(n-2)!*[n-2+(1/2)(1+(-1)^(n-1))(n-1)]
 *            with a(1)=0, a(2)=1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 1;

    seq_t aprev2 = 0;          /* a(n-2) */
    seq_t aprev1 = 1;          /* a(n-1) */
    seq_t fact = 1;            /* (i-2)! for i=3 */
    seq_t a_val = 0;

    for (idx_t i = 3; i <= n; ++i) {
        /* term1 = (2*i-3) * a(n-1) */
        seq_t term1 = ((seq_t)(2 * i - 3)) * aprev1;

        /* term2 = (i-1)*(i-3) * a(n-2) */
        seq_t coeff = ((seq_t)(i - 1)) * (i - 3);
        seq_t term2 = coeff * aprev2;

        /* t3 depends on parity of i */
        seq_t t3;
        if (i & 1U) {          /* i is odd */
            t3 = fact * ((seq_t)(2 * i - 3));
        } else {               /* i is even */
            t3 = fact * ((seq_t)(i - 2));
        }

        /* a(n) = term1 - term2 + t3  (safe unsigned subtraction) */
        a_val = term1 + t3;
        a_val -= term2;        /* term1 + t3 >= term2 for all n >=3 */

        /* shift for next iteration */
        aprev2 = aprev1;
        aprev1 = a_val;

        /* update factorial: (i-2)! -> (i-1)! for next i */
        fact *= ((seq_t)(i - 1));
    }
    return aprev1;
}
