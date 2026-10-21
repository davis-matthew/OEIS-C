/*
 * A070565  n - product of digits of n.
 * https://oeis.org/A070565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, look, nonn
 * Formula:   a(n) = n - product of digits of n
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: a(0) = 0 */
    if (n == 0)
        return 0;

    seq_t prod = 1;
    idx_t m = n;

    while (m > 0) {
        unsigned digit = (unsigned)(m % 10);
        if (digit == 0) {
            prod = 0;
            break;              /* product becomes zero */
        }
        prod *= (seq_t)digit;
        m /= 10;
    }

    return (seq_t)n - prod;
}
