/*
 * A006068  a(n) is Gray-coded into n.
 * https://oeis.org/A006068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, look, hear
 * Formula:   a(n) = n XOR [n/2] XOR [n/4] XOR [n/8] ... XOR [n/2^m] where m =
 *            floor(log2(n))
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n;
    seq_t shifted = (seq_t)n;
    
    while (shifted > 0) {
        shifted >>= 1;
        result ^= shifted;
    }
    
    return result;
}
