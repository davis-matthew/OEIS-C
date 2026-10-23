/*
 * A169931  a(n) = 2*n in the arithmetic defined in A169918.
 * https://oeis.org/A169931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = shift each decimal digit of n by +2 mod 10 (equivalently
 *            a(n) = A048379(A048379(n))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) = 2 by definition */
        return 2;
    }

    /* Count decimal digits of n */
    idx_t tmp = n;
    idx_t len = 0;
    while (tmp != 0) {
        len++;
        tmp /= 10;
    }

    /* Highest power of 10 less than or equal to n */
    idx_t pow10 = 1;
    for (idx_t i = 1; i < len; ++i) {
        pow10 *= 10;
    }

    seq_t result = 0;
    while (pow10 > 0) {
        unsigned digit = (n / pow10) % 10;
        unsigned shifted = (digit + 2) % 10;
        result = result * 10 + shifted;
        pow10 /= 10;
    }

    return result;
}
