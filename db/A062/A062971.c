/*
 * A062971  a(n) = (2*n)^n.
 * https://oeis.org/A062971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 2*n as seq_t */
    seq_t base = (seq_t)n * 2;
    seq_t result = 1;      /* seq_t literal 1 is fine */
    seq_t power = base;
    idx_t exp = n;

    /* binary exponentiation */
    while (exp != 0) {
        if (exp & 1u)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}
