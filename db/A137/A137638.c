/*
 * A137638  Antidiagonal sums of square array A137634.
 * https://oeis.org/A137638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A137638 – Antidiagonal sums of square array A137634
 *
 *  Formula:  a(n) = sum_{k=0..n} sum_{j=0..k} C(n-k+2*j,j) * C(n-k+2*j,k-j)
 *
 *  Recurrence (order 5, polynomial coefficients):
 *      2*(n+1)*a(n) + (-3*n-7)*a(n-1) + 2*(-17*n+10)*a(n-2)
 *    + 8*(-7*n+10)*a(n-3) + 2*(-18*n+37)*a(n-4) + 4*(-2*n+5)*a(n-5) = 0
 *
 *  Initial values:
 *      a(0)=1, a(1)=3, a(2)=15, a(3)=72, a(4)=361
 *
 *  The implementation uses the above recurrence, computing a(n) from
 *  the five preceding terms.  Results are exact whenever they fit in
 *  the chosen seq_t type (arbitrary‑precision or unsigned integral).
 *  The routine works for any call order of n.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms up to a(4).  Returned directly for n < 5. */
    static const seq_t init[5] = { (seq_t)1, (seq_t)3, (seq_t)15,
                                   (seq_t)72, (seq_t)361 };

    if (n < 5)
        return init[n];

    /* Ring buffer holding the last five computed terms:
     * arr[0] = a(i-5), arr[1] = a(i-4), ..., arr[4] = a(i-1).
     */
    seq_t arr[6];
    arr[0] = init[0];
    arr[1] = init[1];
    arr[2] = init[2];
    arr[3] = init[3];
    arr[4] = init[4];
    /* arr[5] is unused; values will wrap around modulo 6. */

    for (idx_t i = 5; i <= n; ++i) {
        /* Indices of required terms in the ring buffer */
        seq_t a_im1 = arr[(i - 1) % 6];
        seq_t a_im2 = arr[(i - 2) % 6];
        seq_t a_im3 = arr[(i - 3) % 6];
        seq_t a_im4 = arr[(i - 4) % 6];
        seq_t a_im5 = arr[(i - 5) % 6];

        /* Compute the numerator of the recurrence:
         *   coeff2*a(im1) + coeff3*a(im2) + ... + coeff6*a(im5)
         */
        seq_t num =
            (seq_t)(3 * i + 7) * a_im1 +
            (seq_t)(2 * (17 * i - 10)) * a_im2 +
            (seq_t)(8 * (7 * i - 10)) * a_im3 +
            (seq_t)(2 * (18 * i - 37)) * a_im4 +
            (seq_t)(4 * (2 * i - 5)) * a_im5;

        /* Denominator: 2*(i+1) */
        seq_t denom = (seq_t)(2 * (i + 1));

        /* Compute a(i) = num / denom */
        arr[i % 6] = num / denom;
    }

    return arr[n % 6];
}
