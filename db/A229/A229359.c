/*
 * A229359  Total sum of n-th powers of parts in all partitions of 8.
 * https://oeis.org/A229359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 45 + 19*2^n + 9*3^n + 6*4^n + 3*5^n + 2*6^n + 7^n + 8^n
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* compute small powers once */
    seq_t pow2n = pow_seq((seq_t)2, (int)n);
    seq_t pow3n = pow_seq((seq_t)3, (int)n);
    seq_t pow4n = pow2n * pow2n;
    seq_t pow5n = pow_seq((seq_t)5, (int)n);
    seq_t pow6n = pow2n * pow3n;
    seq_t pow7n = pow_seq((seq_t)7, (int)n);
    seq_t pow8n = pow2n * pow4n;

    /* assemble the result */
    seq_t result = (seq_t)45;
    result += (seq_t)19 * pow2n;
    result += (seq_t)9 * pow3n;
    result += (seq_t)6 * pow4n;
    result += (seq_t)3 * pow5n;
    result += (seq_t)2 * pow6n;
    result += pow7n;
    result += pow8n;

    return result;
}
