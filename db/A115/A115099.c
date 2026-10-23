/*
 * A115099  a(0)=4, a(n) = 3*a(n-1) - 4.
 * https://oeis.org/A115099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^n + 2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binary exponentiation to compute 3^n */
    seq_t result = (seq_t)1;   /* 3^0 */
    seq_t base   = (seq_t)3;   /* base for exponentiation */
    idx_t exp    = n;          /* copy of the exponent */

    while (exp) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1;
    }

    /* a(n) = 2*3^n + 2 */
    result = result * (seq_t)2 + (seq_t)2;
    return result;
}
