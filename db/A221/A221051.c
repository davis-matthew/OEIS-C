/*
 * A221051  The hyper-Wiener index of the nanostar dendrimer D_1[n], defined
 *          pictorially in the Eslahchi et al. reference.
 * https://oeis.org/A221051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -958 + 2^n*(896*n^2-2904*n-19861) +
 *            4^n*(3136*n^2-11928*n+20890)
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Translate the index to a signed 64‑bit integer for polynomial calculations. */
    int64_t N   = (int64_t)n;
    int64_t nsq = N * N;

    int64_t poly2 = 896 * nsq - 2904 * N - 19861;
    int64_t poly4 = 3136 * nsq - 11928 * N + 20890;

    /* Compute the powers 2^n and 4^n. */
    seq_t two_n  = pow2(n);
    seq_t four_n = pow4(n);

    /* Separate positive and negative contributions to avoid unsigned underflow. */
    seq_t pos = 0, neg = 0;

    if (poly2 >= 0)
        pos += two_n * (seq_t)poly2;
    else
        neg += two_n * (seq_t)(-poly2);

    if (poly4 >= 0)
        pos += four_n * (seq_t)poly4;
    else
        neg += four_n * (seq_t)(-poly4);

    /* constant term -958 */
    neg += (seq_t)958;

    /* pos must be ≥ neg for all n. */
    return pos - neg;
}
