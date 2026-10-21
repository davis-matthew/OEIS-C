/*
 * A089387  Number of Schroeder paths of semilength n (i.e., lattice paths
 *          from (0,0) to (2n,0), with steps H=(2,0), U=(1,1) and D(1,-1) and
 *          not going below the x-axis) with no UD, UHD, UHHD, UHHHD, ...
 *          starting at level zero.
 * https://oeis.org/A089387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies * 2*(n+1)*a(n) + (-17*n+7)*a(n-1) +
 *            (37*n-59)*a(n-2) * + (-37*n+89)*a(n-3) + (17*n-61)*a(n-4) * +
 *            2*(-n+5)*a(n-5) = 0 for n ≥ 5 * Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=8, a(4)=36. * The implementation uses an iterative
 *            recurrence that avoids * negative intermediate terms
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial known values */
    static const seq_t init[5] = { 1, 1, 2, 8, 36 };
    if (n < 5) return init[n];

    /* a(n-5), a(n-4), a(n-3), a(n-2), a(n-1) in this order */
    seq_t prev[5];
    prev[0] = init[0];
    prev[1] = init[1];
    prev[2] = init[2];
    prev[3] = init[3];
    prev[4] = init[4];

    for (idx_t i = 5; i <= n; ++i)
    {
        /* use sequence index i for coefficients */
        /* positive part: (17*i-7)*a(i-1) + (37*i-89)*a(i-3) + (2*i-10)*a(i-5) */
        seq_t pos1 = (seq_t)(17*i - 7)  * prev[4];
        seq_t pos2 = (seq_t)(37*i - 89) * prev[2];
        seq_t pos3 = (seq_t)(2*i - 10)  * prev[0];
        seq_t pos  = pos1 + pos2 + pos3;

        /* negative part: (37*i-59)*a(i-2) + (17*i-61)*a(i-4) */
        seq_t neg1 = (seq_t)(37*i - 59) * prev[3];
        seq_t neg2 = (seq_t)(17*i - 61) * prev[1];
        seq_t neg  = neg1 + neg2;

        /* numerator = pos - neg   (pos ≥ neg for all i ≥ 5) */
        seq_t num = pos - neg;

        /* denominator = 2*(i+1) */
        seq_t den = (seq_t)(2*(i + 1));

        seq_t cur = num / den;

        /* shift window for next iteration:
         * prev[0] <- prev[1]  (a(i-4))
         * prev[1] <- prev[2]  (a(i-3))
         * prev[2] <- prev[3]  (a(i-2))
         * prev[3] <- prev[4]  (a(i-1))
         * prev[4] <- cur      (a(i))   */
        prev[0] = prev[1];
        prev[1] = prev[2];
        prev[2] = prev[3];
        prev[3] = prev[4];
        prev[4] = cur;
    }

    return prev[4];
}
