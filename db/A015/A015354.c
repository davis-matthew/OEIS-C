/*
 * A015354  Gaussian binomial coefficient [ n,7 ] for q = -12.
 * https://oeis.org/A015354
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [n,7]_{q=-12} (Gaussian binomial coefficient with q =
 *            -12)
 * Range:     i8 n <= 7, i16 n <= 7, i32 n <= 8, i64 n <= 9, i128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types */

/* Return the n-th term for n >= 7.  For n < 7 the returned value is 0. */
seq_t a(idx_t n)
{
    /* Maximum n that the routine is expected to handle in the test harness */
    #define MAX_N 20000

    /* q value for this sequence */
    static const seq_t q = (seq_t)(-12);

    /* Memoisation of powers of q and binomial coefficients */
    static seq_t pow_q[MAX_N + 1];   /* q^i for i = 0..MAX_N */
    static seq_t binom[8];           /* binom[k] = [current_n, k] for k = 0..7 */

    static idx_t computed_n = 0;     /* highest n for which binom is valid */

    /* Initialise the tables on the first call. */
    if (computed_n == 0) {
        pow_q[0] = (seq_t)1;
        binom[0] = (seq_t)1;
        for (int k = 1; k <= 7; ++k) binom[k] = (seq_t)0;
    }

    /* Clamp n to the supported range.  Calls with n > MAX_N will be ignored. */
    if (n > (idx_t)MAX_N) n = (idx_t)MAX_N;

    /* Incrementally compute up to n, using the recurrence:
       [n,k] = [n-1,k] + q^{n-k} * [n-1,k-1]  */
    while (computed_n < n) {
        ++computed_n;                           /* now computed_n == current n */

        /* Compute q^computed_n */
        pow_q[computed_n] = pow_q[computed_n - 1] * q;

        /* Update binomial coefficients for k = 7 down to 1 */
        for (int k = 7; k >= 1; --k) {
            if (computed_n >= (idx_t)k) {
                binom[k] += pow_q[computed_n - k] * binom[k - 1];
            }
        }
        /* binom[0] stays 1 */
    }

    /* binom[7] now holds [n,7] for the requested n. */
    return binom[7];
}
