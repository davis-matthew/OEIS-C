/*
 * A109523  a(n) is the sum of the (1,2)- and (1,3)-entries of the matrix P^n
 *          + T^n, where the 3 X 3 matrices P and T are defined by P = [0,1,0;
 *          0,0,1; 1,0,0] and T = [0,1,0; 0,0,1; 1,1,1].
 * https://oeis.org/A109523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = u(n) + ((n mod 3) != 0), u(0)=0, u(1)=1, u(2)=1, u(n) =
 *            u(n-1) + u(n-2) + u(n-3)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* u_0 = 0, u_1 = 1, u_2 = 1 */
    seq_t u_prev2 = 0; /* u_{i-2} */
    seq_t u_prev1 = 1; /* u_{i-1} */
    seq_t u_cur   = 1; /* u_i */

    if (n == 0) return u_prev2 + ((n % 3) ? 1 : 0);
    if (n == 1) return u_prev1 + ((n % 3) ? 1 : 0);
    if (n == 2) return u_cur   + ((n % 3) ? 1 : 0);

    for (idx_t i = 3; i <= n; ++i) {
        seq_t u_next = u_cur + u_prev1 + u_prev2;
        u_prev2 = u_prev1;
        u_prev1 = u_cur;
        u_cur   = u_next;
    }

    return u_cur + ((n % 3) ? 1 : 0);
}
