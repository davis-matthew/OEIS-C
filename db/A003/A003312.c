/*
 * A003312  a(1) = 3; for n>0, a(n+1) = a(n) + floor((a(n)-1)/2).
 * https://oeis.org/A003312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 3; a(n+1) = a(n) + floor((a(n)-1)/2)
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 218, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)3;
    for (idx_t i = 1; i < n; i++) {
        result = result + (result - 1) / 2;
    }
    return result;
}
