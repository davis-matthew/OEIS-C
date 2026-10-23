/*
 * A138908  a(n) = d^d, where d is the number of digits in n.
 * https://oeis.org/A138908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = d^d, where d is the number of decimal digits in n
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count decimal digits of n (0 has one digit) */
    int d;
    if (n == 0)
        d = 1;
    else {
        uint64_t temp = (uint64_t)n;
        d = 0;
        while (temp) {
            temp /= 10;
            ++d;
        }
    }

    /* Compute d^d */
    seq_t base = (seq_t)d;
    seq_t result = 1;
    for (int i = 0; i < d; ++i)
        result *= base;

    return result;
}
