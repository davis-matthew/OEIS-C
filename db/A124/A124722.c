/*
 * A124722  Number of ternary Lyndon words with exactly four 1's.
 * https://oeis.org/A124722
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ( ( (n-4+3 choose 3)*2^{n-4} - [n-4 even?
 *            ((n-4)/2+1)*2^{(n-4)/2} : 0 ] ) ) / 4 The sequence starts with
 *            a(5)=2, a(6)=9, a(7)=40, a(8)=137,
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle indices below the offset – behaviour is arbitrary */
    if (n < 5) return 0;

    /* shift index to start at 1 for formula */
    idx_t np = n - 4;               /* np >= 1 */

    /* binomial (np+3 choose 3) = (np+3)(np+2)(np+1) / 6 */
    seq_t binom = (((seq_t)(np + 1)) * ((seq_t)(np + 2)) * ((seq_t)(np + 3))) / (seq_t)6;

    /* 2^np */
    seq_t pow_np = pow2(np);

    /* first term: binom * 2^np / 4 */
    seq_t term1 = (binom * pow_np) / (seq_t)4;

    /* second term, only if np is even */
    seq_t term2 = 0;
    if ((np & 1) == 0) {
        idx_t half = np >> 1;           /* np / 2 */
        seq_t mult  = (seq_t)(half + 1);/* (np/2)+1 */
        seq_t pow_half = pow2(half);    /* 2^(np/2) */
        term2 = (mult * pow_half) / (seq_t)4;
    }

    return term1 - term2;
}
