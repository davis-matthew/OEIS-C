/*
 * A166818  Number of nX2 1..7 arrays containing at least one of each value,
 *          all equal values connected, rows considered as a single number in
 *          nondecreasing order, and columns considered as a single number in
 *          increasing order.
 * https://oeis.org/A166818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = ((2^5*4!)/(12)!)*binomial(n,4)*P(n) where P(n) = n^8 +
 *            78n^7 + 3405n^6 + 93498n^5 + 1514529n^4 + 12368202n^3 +
 *            25076435n^2 – 107706168n – 3335760
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* binomial(n,4) is zero for n < 4, so the whole expression is zero. */
    seq_t comb = binom(n, 4);          /* (n choose 4) */

    /* Horner's rule to evaluate the degree‑8 polynomial P(n). */
    seq_t poly = 1;                    /* leading coefficient of n^8 */

    poly = poly * n + 78;
    poly = poly * n + 3405;
    poly = poly * n + 93498;
    poly = poly * n + 1514529;
    poly = poly * n + 12368202;
    poly = poly * n + 25076435;
    poly = poly * n - 107706168;
    poly = poly * n - 3335760;

    seq_t num  = comb * poly;          /* numerator before division */
    const seq_t denom = 623700;        /* 12!/(2^5*4!) = 623700 */

    return num / denom;
}
