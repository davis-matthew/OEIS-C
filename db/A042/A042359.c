/*
 * A042359  Denominators of continued fraction convergents to sqrt(706).
 * https://oeis.org/A042359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = denominator of nth convergent of the simple continued fraction of sqrt(706).
 * Recurrence:  let p[-2]=0, p[-1]=1, q[-2]=1, q[-1]=0.
 *              For k>=0: p[k]=a[k]*p[k-1]+p[k-2] , q[k]=a[k]*q[k-1]+q[k-2]
 *              where a[0]=26 and for k>=1 a[k] repeats the period [1,1,3,26,3,1,1,52].
 * Initial values: a(0)=1, a(1)=1, a(2)=2, a(3)=7, ...
 */
#include <stdint.h>
#include <stdbool.h>

static const seq_t period[8] = {1, 1, 3, 26, 3, 1, 1, 52};
static const seq_t a0 = 26;

seq_t a(idx_t n)
{
    // Iterative computation of convergents
    seq_t p_minus2 = 0;   // p[-2]
    seq_t p_minus1 = 1;   // p[-1]
    seq_t q_minus2 = 1;   // q[-2]
    seq_t q_minus1 = 0;   // q[-1]
    seq_t p_cur, q_cur;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t ak;
        if (i == 0) {
            ak = a0;
        } else {
            ak = period[(i - 1) % 8];
        }
        p_cur = ak * p_minus1 + p_minus2;
        q_cur = ak * q_minus1 + q_minus2;
        if (i == n) {
            return q_cur;
        }
        // shift for next iteration
        p_minus2 = p_minus1;
        p_minus1 = p_cur;
        q_minus2 = q_minus1;
        q_minus1 = q_cur;
    }
    // Should never reach here
    return 0;
}
