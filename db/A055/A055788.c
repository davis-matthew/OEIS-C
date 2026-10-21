/*
 * A055788  Number of symmetric types of (5,2n)-hypergraphs under action of
 *          complementing group C(5,2).
 * https://oeis.org/A055788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (62*C(n+15,15) - 310*C(n/2+7,7) + 1240*C(n/4+3,3) -
 *            1984*(n/8+1) + 1024*indicator(n%16==0)) / 32 (with terms applied
 *            only when the divisibility condition holds)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* first term: always present */
    sum += (seq_t)62 * binomial(n + 15, 15);

    /* second term: n even */
    if ((n & 1) == 0) {
        idx_t j = n >> 1;          /* j = n/2 */
        sum -= (seq_t)310 * binomial(j + 7, 7);
    }

    /* third term: n divisible by 4 */
    if ((n & 3) == 0) {
        idx_t j = n >> 2;          /* j = n/4 */
        sum += (seq_t)1240 * binomial(j + 3, 3);
    }

    /* fourth term: n divisible by 8 */
    if ((n & 7) == 0) {
        idx_t j = n >> 3;          /* j = n/8 */
        sum -= (seq_t)1984 * ((seq_t)j + 1);
    }

    /* fifth term: n divisible by 16 */
    if ((n & 15) == 0) {
        sum += (seq_t)1024;
    }

    return sum / 32;                /* exact division by 32 */
}
