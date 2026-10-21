/*
 * A228710  Characteristic function of numbers with alternating parities of
 *          their digits in decimal representation.
 * https://oeis.org/A228710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if decimal digits of n alternate in parity, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t prev = n % 10 & 1;   /* parity of least significant digit */
    n /= 10;
    while (n > 0) {
        idx_t curr = n % 10 & 1;   /* parity of next digit to the left */
        if (curr == prev) {
            return 0;
        }
        prev = curr;
        n /= 10;
    }
    return 1;
}
