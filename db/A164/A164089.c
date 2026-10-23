/*
 * A164089  For n >=4, a(n) = the numerical value of the substring of binary n
 *          containing all digits but the first and last. a(1) = a(2) = a(3) =
 *          0.
 * https://oeis.org/A164089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the numeric value of the substring of the binary *
 *            representation of n consisting of all digits except the first *
 *            and last. For n <= 3 the value is 0
 * Range:     u8 n <= 1535, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n <= 3 the binary representation has at most two digits,
     * so the middle substring is empty. */
    if (n <= 3)
        return 0;

    /* Compute the number of bits needed to represent n.
     * Because idx_t is at most uint64_t, the loop will finish in
     * at most 64 iterations. */
    idx_t tmp = n;
    int bits = 0;
    while (tmp) {
        bits++;
        tmp >>= 1;
    }

    /* bits >= 3 for n >= 4.  The number of bits in the middle
     * substring is bits - 2. */
    int mid_bits = bits - 2;

    /* Build a mask that has the lowest mid_bits bits set to 1.
     * (seq_t)1 is cast to seq_t before shifting so that the result
     * matches the width of seq_t. */
    seq_t mask = ((seq_t)1 << mid_bits) - 1;

    /* n >> 1 removes the last (least significant) bit.
     * Masking with 'mask' removes the leading bit of the original,
     * leaving only the desired middle bits. */
    seq_t result = ((seq_t)(n >> 1)) & mask;
    return result;
}
