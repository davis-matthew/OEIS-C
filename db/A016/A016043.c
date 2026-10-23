/*
 * A016043  2^(2^n) +- 1 without repeats.
 * https://oeis.org/A016043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n >= 2: let k = ⌊n/2⌋. a(2k) = 2^{2^k} – 1 (even
 *            indices) a(2k+1) = 2^{2^k} + 1 (odd indices)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   pow2, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    if (n == 1) {          /* special case 1 = 2^1 – 1 */
        return (seq_t)1;
    }

    /* k = floor(n/2)  (integer division) */
    idx_t k = n / 2;

    /* 2^k as a seq_t (needed for the exponent of the next power) */
    seq_t pow2k = pow2(k);

    /* 2^{2^k}  */
    seq_t val = pow_seq_large((seq_t)2, pow2k);

    if (n & 1U) {          /* odd n => +1 */
        return val + (seq_t)1;
    } else {               /* even n => -1 */
        return val - (seq_t)1;
    }
}
