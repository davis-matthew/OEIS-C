/*
 * A032197  Number of necklaces with 12 black beads and n-12 white beads.
 * https://oeis.org/A032197
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/12 * ( binom(12 + (n-12) - 1, n-12) + binom(6 +
 *            ((n-12)/2) - 1, (n-12)/2) if (n-12) even + 2 * binom(4 +
 *            ((n-12)/3) - 1, (n-12)/3) if (n-12) % 3 == 0 + 2 * binom(3 +
 *            ((n-12)/4) - 1, (n-12)/4) if (n-12) % 4 == 0 + 2 * binom(2 +
 *            ((n-12)/6) - 1, (n-12)/6) if (n-12) % 6 == 0 + 4 * binom(1 +...
 * Range:     u8 n <= 14, u16 n <= 19, u32 n <= 35, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binom_from_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */
#include "../utils/binom_from_t.h"

seq_t a(idx_t n)
{
    if (n < 12)          /* undefined for n < 12, return 0 */
        return (seq_t)0;

    idx_t k = n - 12;    /* excess white beads beyond the 12 black ones */
    seq_t sum = (seq_t)0;

    /* Term d=1, m=12, weight=1 */
    if (k >= 0) {
        sum += binom_from_t(k, (idx_t)11);
    }

    /* d=2, m=6, weight=1 */
    if ((k & 1) == 0) { /* k even */
        idx_t t = k >> 1;
        sum += binom_from_t(t, (idx_t)5);
    }

    /* d=3, m=4, weight=2 */
    if (k % 3 == 0) {
        idx_t t = k / 3;
        sum += (seq_t)2 * binom_from_t(t, (idx_t)3);
    }

    /* d=4, m=3, weight=2 */
    if (k % 4 == 0) {
        idx_t t = k / 4;
        sum += (seq_t)2 * binom_from_t(t, (idx_t)2);
    }

    /* d=6, m=2, weight=2 */
    if (k % 6 == 0) {
        idx_t t = k / 6;
        sum += (seq_t)2 * binom_from_t(t, (idx_t)1);
    }

    /* d=12, m=1, weight=4 */
    if (k % 12 == 0) {
        idx_t t = k / 12;
        sum += (seq_t)4 * binom_from_t(t, (idx_t)0); /* binom(t,0) = 1 */
    }

    return sum / (seq_t)12;
}
