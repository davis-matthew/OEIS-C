/*
 * A164583  a(n) = 4^n*(2n + 1)^2.
 * https://oeis.org/A164583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n*(2n+1)^2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;   /* 4^n will be accumulated here */
    seq_t base   = 4;   /* current power of 4 */
    idx_t exp    = n;   /* exponent in uint64_t */

    /* fast exponentiation */
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* multiply by (2n+1)^2 */
    seq_t t = (seq_t)(2 * n + 1);
    result *= t;
    result *= t;

    return result;
}
