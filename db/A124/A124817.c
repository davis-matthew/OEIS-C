/*
 * A124817  Interpolation of A001764(n+1) and A006629(n).
 * https://oeis.org/A124817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2k) = binomial(3k+3, k+1)/(2k+3) a(2k+1) = 2*binomial(3k+3,
 *            k)/(k+2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if ((n & 1) == 0) {                    /* even n : n = 2k */
        idx_t k      = n >> 1;              /* k = n/2 */
        idx_t N      = 3 * k + 3;           /* n = 3k+3 */
        idx_t K      = k + 1;               /* choose k+1 */
        seq_t bin    = binomial(N, K);      /* C(3k+3, k+1) */
        seq_t denom  = (seq_t)(2 * k + 3);  /* 2k+3 */
        return bin / denom;
    } else {                                /* odd n : n = 2k+1 */
        idx_t k      = n >> 1;              /* k = floor((n-1)/2) */
        idx_t N      = 3 * k + 3;           /* n = 3k+3 */
        idx_t K      = k;                   /* choose k */
        seq_t bin    = binomial(N, K);      /* C(3k+3, k) */
        seq_t denom  = (seq_t)(k + 2);      /* k+2 */
        return (bin * 2) / denom;
    }
}
