/*
 * A032194  Number of necklaces with 9 black beads and n-9 white beads.
 * https://oeis.org/A032194
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (C(n-1,8) + 2*C((n-9)/3 + 2, 2) if (n-9)%3==0 else 0 * +
 *            6*1 if (n-9)%9==0 else 0) / 9 * where C(a,b) is the binomial
 *            coefficient
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for idx_t, seq_t */
#include "../utils/binomial.h" /* assumes binomial(idx_t, idx_t) is available */

seq_t a(idx_t n) {
    if (n < 9) return 0;

    /* Term from 1/(1-x)^9 */
    seq_t term1 = binomial(n - 1, 8);

    /* Term from 2/(1-x^3)^3 */
    seq_t term2 = 0;
    if ((n - 9) % 3 == 0) {
        idx_t k = (n - 9) / 3;          /* k = (n-9)/3 */
        idx_t a2 = k + 2;               /* a2 = k+2 for binomial(a2,2) */
        term2 = 2 * binomial(a2, 2);    /* multiply by 2 */
    }

    /* Term from 6/(1-x^9) */
    seq_t term3 = 0;
    if ((n - 9) % 9 == 0) {
        term3 = 6;                      /* 6*1 */
    }

    /* Combine and divide by 9 */
    seq_t sum = term1 + term2 + term3;
    return sum / 9;
}
