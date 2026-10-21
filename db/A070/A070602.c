/*
 * A070602  n^5 mod 18.
 * https://oeis.org/A070602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 mod 18
 * Range:     u8 n <= 88, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod = 18;
    seq_t r = (seq_t)n % mod;   /* reduce n modulo 18 */
    seq_t result = 1;
    for (int i = 0; i < 5; ++i) {
        result = (result * r) % mod;   /* multiply and reduce each step */
    }
    return result;
}
