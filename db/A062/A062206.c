/*
 * A062206  a(n) = n^(2n).
 * https://oeis.org/A062206
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^(2n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the arithmetic type */
    seq_t base = n;
    idx_t exp = 2 * n;          /* 2n, fits in idx_t up to 10000 */

    /* Exponentiation by squaring on seq_t */
    seq_t result = 1;
    seq_t power = base;

    while (exp) {
        if (exp & 1) result *= power;
        power *= power;
        exp >>= 1;
    }

    return result;
}
