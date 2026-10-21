/*
 * A082685  a(n) = (2*5^n + 2^n)/3.
 * https://oeis.org/A082685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*5^n + 2^n)/3, offset = 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = (seq_t)1;
    seq_t pow2 = (seq_t)1;

    /* Compute 5^n and 2^n using only seq_t operations. */
    for (idx_t i = (idx_t)0; i < n; ++i) {
        pow5 *= (seq_t)5;
        pow2 *= (seq_t)2;
    }

    /* Compute (2*5^n + 2^n)/3 exactly. */
    seq_t numerator = (pow5 * (seq_t)2) + pow2;
    seq_t result = numerator / (seq_t)3;

    return result;
}
