/*
 * A229267  Number of lattice paths from {n}^n to {0}^n using steps that
 *          decrement one component or all components by 1.
 * https://oeis.org/A229267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = Sum_{j=0..n} (n+(n-1)*j)! / ((n-j)! * (j!)^n)
 *   (n > 1, a(0) = a(1) = 1)
 *
 * The implementation evaluates the sum by successive multiplicative
 * updates, keeping only a few large intermediate integers and using
 * only integer arithmetic.  The function works for any integer type
 * seq_t provided by the build (uint8_t ... uint64_t, arbitrary‑precision
 * integer, etc.) and respects the constraints from the problem
 * statement.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0 || n == 1) {
        return 1;
    }

    /* Current term of the sum (f_j) and cumulative sum */
    seq_t f = 1;    /* f_0 = 1 */
    seq_t sum = 0;

    /* W_j = product_{k=1}^{n-1} (t_j + k) where t_j = n + (n-1)*j
     * For j = 0, t_0 = n, so W_0 = ∏_{k=1}^{n-1} (n + k) = ∏_{i=n+1}^{2n-1} i   */
    seq_t W = 1;
    for (idx_t k = 1; k < n; ++k) {
        W *= (seq_t)(n + k);
    }

    /* Main loop: compute terms f_0, f_1, …, f_n and accumulate the sum */
    for (idx_t j = 0; j < n; ++j) {
        sum += f;  /* add current term a_j */

        /* Compute f_{j+1} = f_j * (n-j) * W_j / (j+1)^n
         * First multiply, then perform n successive integer divisions by (j+1) */
        seq_t tmp = f * (seq_t)(n - j) * W;
        for (idx_t k = 0; k < n; ++k) {
            tmp /= (seq_t)(j + 1);
        }
        f = tmp;

        /* Update W for the next iteration:
         * W_{j+1} = ∏_{k=1}^{n-1} (t_{j+1} + k)
         *        = ∏_{k=n}^{2n-2} (t_j + k)
         * We slide the window by dividing out the first n-1 terms and
         * multiplying in the new n-1 terms. */
        idx_t t_j = n + (n - 1) * j;   /* t_j = n + (n-1)*j */

        for (idx_t k = 1; k < n; ++k) {
            W /= (seq_t)(t_j + k);          /* remove old factor */
        }
        for (idx_t k = n; k <= 2 * n - 2; ++k) {
            W *= (seq_t)(t_j + k);          /* add new factor   */
        }
    }

    /* Append the last term f_n to the sum */
    sum += f;

    return sum;
}
