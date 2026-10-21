/*
 * A088662  Number of peaks at even level in all symmetric Dyck paths of
 *          semilength n+2.
 * https://oeis.org/A088662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = (2n)! * (2n²+4n+1) / [n! (n+1)!] a(2n+1) = 2 * (2n+1)! /
 *            (n!²) which can be rewritten with binomial coefficients as:
 *            a(2n) = binom(2n, n)/(n+1) * (2n²+4n+1) a(2n+1) = 2 * (n+1) *
 *            binom(2n+1, n)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* even index: n = 2k */
    if ((n & 1) == 0) {
        idx_t k = n >> 1;                    /* k = n/2                 */
        seq_t bin = binom(2 * k, k);         /* C(2k, k)                */
        seq_t denom = (seq_t)k + 1;          /* k+1                     */
        seq_t q = bin / denom;               /* bin/(k+1)               */

        /* compute 2k^2 + 4k + 1 */
        seq_t kk = (seq_t)k * (seq_t)k;      /* k²                       */
        seq_t num = (seq_t)2 * kk + (seq_t)4 * (seq_t)k + (seq_t)1;

        return q * num;
    }

    /* odd index: n = 2k+1 */
    idx_t k = n >> 1;                        /* k = (n-1)/2             */
    seq_t bin = binom(2 * k + 1, k);         /* C(2k+1, k)              */
    seq_t kp1 = (seq_t)k + 1;                /* k+1                     */

    return (seq_t)2 * kp1 * bin;
}
