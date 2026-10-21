/*
 * A074359  Coefficient of q^2 in nu(n), where nu(0)=1, nu(1)=b and, for n>=2,
 *          nu(n)=b*nu(n-1)+lambda*(1+q+q^2+...+q^(n-2))*nu(n-2) with
 *          (b,lambda)=(2,2).
 * https://oeis.org/A074359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: coefficient of q^2 in the polynomials nu(n) defined by
   nu(0)=1, nu(1)=2, and for n>=2
          nu(n)=2*nu(n-1) + 2*(1+q+...+q^(n-2))*nu(n-2).
   The coefficients of q^0 and q^1 in nu(n) (denoted c0(n) and c1(n))
   satisfy the linear recurrences
          c0(0)=1,   c0(1)=2,   c0(n)=2*c0(n-1)+2*c0(n-2),
          c1(0)=0,   c1(1)=0,   c1(2)=0,   c1(3)=4,
          c1(n)=2*c1(n-1)+2*(c1(n-2)+c0(n-2))  (n>=4).
   Consequently, the coefficient of q^2, a(n)=c2(n), obeys
          a(0)=a(1)=a(2)=a(3)=0,
          a(n)=2*a(n-1)+2*(c0(n-2)+c1(n-2)+a(n-2))  (n>=4).
   The implementation iteratively evaluates these three simultaneous
   recurrences and returns a(n). */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;

    /* Initial values for n=2 and n=3 */
    seq_t c0_prev2 = 6;  /* c0(2)  */
    seq_t c0_prev1 = 16; /* c0(3)  */
    seq_t c1_prev2 = 0;  /* c1(2)  */
    seq_t c1_prev1 = 4;  /* c1(3)  */
    seq_t c2_prev2 = 0;  /* c2(2)  */
    seq_t c2_prev1 = 0;  /* c2(3)  */

    seq_t c0_curr, c1_curr, c2_curr;

    for (idx_t i = 4; i <= n; ++i) {
        c0_curr = 2 * c0_prev1 + 2 * c0_prev2;
        c1_curr = 2 * c1_prev1 + 2 * (c1_prev2 + c0_prev2);
        c2_curr = 2 * c2_prev1 + 2 * (c2_prev2 + c1_prev2 + c0_prev2);

        c0_prev2 = c0_prev1;
        c0_prev1 = c0_curr;
        c1_prev2 = c1_prev1;
        c1_prev1 = c1_curr;
        c2_prev2 = c2_prev1;
        c2_prev1 = c2_curr;
    }
    return c2_prev1;
}
