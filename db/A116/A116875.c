/*
 * A116875  Generalized Catalan numbers C(2,5;n)=C(5,2;n).
 * https://oeis.org/A116875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(2,5;n), with generating function *
 *            (2+13*x+30*x^2*c(10*x))/((1+5*x)*(2+x)), where c(x) is the
 *            Catalan * generating function. * The sequence satisfies * *
 *            2*a(n) + 11*a(n-1) + 5*a(n-2) = * 30 * Catalan(n-2) * 10^(n-2)
 *            (for n >= 2) * * Initial values: * a(0) = 1 * a(1) = 1 * * This
 *            impleme...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for NULL if needed */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0 || n == 1)
        return 1;

    /* Variables for the recurrence */
    seq_t a_prev2 = 1;   /* a(0) */
    seq_t a_prev1 = 1;   /* a(1) */
    seq_t a_cur   = 0;   /* current a(n) */

    /* Catalan number C(k) for k = current (i-2) */
    seq_t cat_k   = 1;   /* Catalan(0) at i = 2 */

    /* Power of 10: 10^(i-2) */
    seq_t pow10   = 1;   /* 10^0 at i = 2 */

    /* Main loop: compute a(2) to a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        /* term = 30 * cat_k * pow10 - 11*a_prev1 - 5*a_prev2 */
        seq_t term = 30 * cat_k * pow10;
        term = term - 11 * a_prev1;
        term = term - 5 * a_prev2;
        a_cur = term / 2;  /* division is exact */

        if (i == n)
            return a_cur;

        /* Update for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;

        /* Update cat_k to Catalan(i-1) */
        /* cat_{k+1} = cat_k * (4*(i-1)-2) / i = cat_k * (4*i-6) / i */
        seq_t numerator = cat_k * (4 * i - 6);
        cat_k = numerator / i;

        /* Update power of 10: multiply by 10 */
        pow10 = pow10 * 10;
    }

    /* Unreachable, but keep compiler happy */
    return a_cur;
}
