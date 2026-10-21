/*
 * A224542  Number of doubly-surjective functions f:[n]->[4].
 * https://oeis.org/A224542
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            4^n-4*3^n-4*n*3^(n-1)+(9*n+3*n^2)*2^(n-1)+6*2^n-4-8*n-4*n^3;
 * Range:     u8 none, u16 n <= 9, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   pow2, pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* For n < 8 the sequence is not defined - return 0 */
    if (n < 8) {
        return 0;
    }

    /* Basic powers */
    seq_t pow4_n  = pow4(n);                       /* 4^n   */
    seq_t pow3_n  = pow3(n);                       /* 3^n   */
    seq_t pow3_n1 = pow3(n - 1);                   /* 3^(n-1) */
    seq_t pow2_n  = pow2(n);                       /* 2^n   */
    seq_t pow2_n1 = pow2(n - 1);                   /* 2^(n-1) */

    seq_t nn = (seq_t)n;                           /* n */

    /* Compute the formula using only integer arithmetic */
    seq_t result = pow4_n;                         /* 4^n */

    /* -4*3^n */
    result -= 4 * pow3_n;

    /* -4*n*3^(n-1) */
    result -= 4 * (nn * pow3_n1);

    /* +(9*n + 3*n^2)*2^(n-1) */
    seq_t tmp = 9 * nn + 3 * nn * nn;
    result += tmp * pow2_n1;

    /* +6*2^n */
    result += 6 * pow2_n;

    /* -4 - 8*n - 4*n^3 */
    result -= 4;
    result -= 8 * nn;
    result -= 4 * nn * nn * nn;

    return result;
}
