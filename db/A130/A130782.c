/*
 * A130782  Period 5, repeat [1, 1, 2, 1, 1].
 * https://oeis.org/A130782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+3)/5) - ceil((n-7)/5) (period 5: [1,1,2,1,1])
 * Range:     u8 n <= 4999, u16 n <= 4999, u32 n <= 4999, u64 n <= 4999,
 *            u128 n <= 4999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Periodic pattern of the sequence with period 5 */
    static const seq_t pattern[5] = { (seq_t)1, (seq_t)1, (seq_t)2, (seq_t)1, (seq_t)1 };

    /* n % 5 fits in idx_t; array indexing is safe */
    return pattern[n % 5];
}
