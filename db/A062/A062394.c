/*
 * A062394  a(n) = 6^n + 1.
 * https://oeis.org/A062394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6^n + 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Exponentiation by squaring: compute 6^n */
    seq_t result = 1;        /* accumulator */
    seq_t base   = 6;        /* current power of 6 */
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            result = result * base;
        }
        base = base * base;
        exp >>= 1;
    }

    return result + 1;
}
