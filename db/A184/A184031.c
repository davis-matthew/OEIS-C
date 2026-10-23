/*
 * A184031  1/16 the number of (n+1) X 2 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   For n even: a(n) = 9*2^(n/2-1) + 9*2^(n-1) + 1 For n odd: a(n) =
 *            9*2^(n-1) + 3*2^((n+1)/2) + 1 (as conjectured from the OEIS
 *            entry)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Protect against n < 1 – offset is 1; return 0 for safety. */
    if (n < 1) {
        return 0;
    }

    /* Common terms */
    seq_t pow_n_minus_1 = pow2(n - 1);

    if ((n & 1) == 0) {
        /* n is even */
        seq_t pow_half_minus_1 = pow2(n / 2 - 1);
        return (seq_t)9 * pow_half_minus_1 + (seq_t)9 * pow_n_minus_1 + (seq_t)1;
    } else {
        /* n is odd */
        seq_t pow_half_plus_half = pow2((n + 1) / 2);
        return (seq_t)9 * pow_n_minus_1 + (seq_t)3 * pow_half_plus_half + (seq_t)1;
    }
}
