/*
 * A199209  a(n) = 10*4^n+1.
 * https://oeis.org/A199209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*4^n+1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;          /* 4^0 = 1 */
    seq_t base   = 4;          /* base for exponentiation */
    idx_t e = n;               /* use a local copy of n for shifting */

    /* Binary exponentiation: compute base^e efficiently */
    while (e) {
        if (e & 1) result *= base;   /* if the current bit is set, multiply result */
        e >>= 1;                     /* shift exponent */
        if (e) base *= base;         /* square base for next bit, skip at end to avoid overflow */
    }

    return result * (seq_t)10 + (seq_t)1;
}
