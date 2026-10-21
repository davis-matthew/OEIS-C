/*
 * A214863  Numbers n such that n XOR 11 = n - 11.
 * https://oeis.org/A214863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 8*n - 2*(-1)^n (offset 1)
 * Range:     u8 n <= 32, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 8;
    if (n & 1) {
        val += 3;   /* odd n: a(n) = 8*n + 3 */
    } else {
        val -= 1;   /* even n: a(n) = 8*n - 1 */
    }
    return val;
}
