/*
 * A117814  a(n) = 1 if at least one of decimal digits of n is a prime,
 *          otherwise a(n)=0.
 * https://oeis.org/A117814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 1 if the decimal expansion of n contains at least one
 *            prime digit (2, 3, 5, or 7); otherwise a(n) = 0
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n;
    while (m > 0) {
        idx_t d = m % 10;
        if (d == 2 || d == 3 || d == 5 || d == 7) {
            return (seq_t)1;
        }
        m /= 10;
    }
    return (seq_t)0;
}
