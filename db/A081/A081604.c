/*
 * A081604  Number of digits in ternary representation of n.
 * https://oeis.org/A081604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(log_3(n)) + 1 for n >= 1, a(0) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Number of digits of n in base 3.
       For n == 0 the ternary representation is "0".   */
    if (n == 0) return 1;

    seq_t digits = 1;          /* at least one digit                     */
    idx_t t = n;
    /* Divide by 3 until n < 3, incrementing the digit count.   */
    while (t > 2) {
        t /= 3;
        digits++;
    }
    return digits;
}
