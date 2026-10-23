/*
 * A172336  a(n) = floor(n*(sqrt(13)+sqrt(2))).
 * https://oeis.org/A172336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor(n*(sqrt(13)+sqrt(2)))
 * Range:     u8 none, u16 n <= 0, u32 n <= 8, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /*  High‑precision rational approximation of sqr(13)+sqr(2)
     *  α = sqrt(13)+sqrt(2) ≈ 5.019764837837084594...
     *  We use the fraction 501976484/100000000, whose error
     *  abs(α - 501976484/100000000) < 2.2*10⁻⁹.
     *  For all n ≤ 10⁴ this gives |n*α - n*(501976484/100000000)| < 0.00005,
     *  so floor(n*α) can differ from the floor of the approximation
     *  by at most 1.  The following test corrects the possible +1 case.
     */
    static const seq_t P = 501976484;          /* numerator of the approximation */
    static const seq_t Q = 100000000;          /* denominator                    */
    static const seq_t THRESH = Q - 5000;      /* 0.99995 * Q, see comment above */

    seq_t prod = (seq_t)n * P;                /* n * P  (exact in seq_t)      */
    seq_t res0  = prod / Q;                   /* floor( n * P / Q )           */
    seq_t rem   = prod % Q;                   /* remainder of the division    */

    /* If the fractional part of the approximation is within 0.00005 of 1,
     * the true value might cross the integer boundary and therefore
     * floor(n*α) = res0 + 1.  Otherwise the floor is res0.
     */
    if (rem >= THRESH)
        res0 += 1;

    return res0;
}
