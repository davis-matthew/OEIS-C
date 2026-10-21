/*
 * A058389  Number of 3 X 3 matrices with nonnegative integer entries and all
 *          row sums equal to n, up to row and column permutation.
 * https://oeis.org/A058389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   a(n) = (1/6)*(C(C(n + 2, 2) + 2, 3) + (3/2)*floor((n +
 *            2)/2)*(C(n + 2, 2) - floor((n + 2)/2)) + 3*C(floor((n + 2)/2) +
 *            2, 3) + 2*floor(C(n + 2, 2)/3) + 2*C(C(n + 2, 2) - 3*floor(C(n +
 *            2, 2)/3) + 2, 3))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* C(n+2,2) = (n+2)(n+1)/2 */
    seq_t S = (seq_t)(n + 2) * (seq_t)(n + 1) / 2;

    idx_t half = (n + 2) / 2;        /* floor((n+2)/2) */

    /* term1 = C(S + 2, 3) */
    seq_t term1 = binom((idx_t)(S + 2), 3);

    /* term2 = (3/2)*half*(S - half) */
    seq_t term2 = ( (seq_t)3 * (seq_t)half * (S - (seq_t)half) ) / 2;

    /* term3 = 3 * C(half + 2, 3) */
    seq_t term3 = (seq_t)3 * binom((idx_t)(half + 2), 3);

    /* term4 = 2 * floor(S / 3) */
    seq_t term4 = (seq_t)2 * (S / 3);

    /* term5 = 2 * C(S - 3*floor(S/3) + 2, 3) */
    seq_t term5 = (seq_t)2 * binom((idx_t)( S - 3 * (S / 3) + 2 ), 3);

    seq_t sum = term1 + term2 + term3 + term4 + term5;

    return sum / 6;
}
