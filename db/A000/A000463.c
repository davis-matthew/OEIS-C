/*
 * A000463  n followed by n^2.
 * https://oeis.org/A000463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = if(n mod 2, (n+1)/2, (n/2)^2)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {
        // n is odd: return (n+1)/2
        return ((seq_t)n + 1) / 2;
    } else {
        // n is even: return (n/2)^2
        seq_t k = (seq_t)n / 2;
        return k * k;
    }
}
