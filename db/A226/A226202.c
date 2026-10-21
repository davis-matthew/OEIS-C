/*
 * A226202  a(n) = 9^n + n.
 * https://oeis.org/A226202
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow9 = 1;          /* 9^0 = 1 */
    seq_t base = 9;          /* current base (9^(2^k)) */
    idx_t e = n;             /* exponent */

    /* fast exponentiation by squaring */
    while (e != 0) {
        if (e & 1)          /* if LSB of e is 1 */
            pow9 *= base;   /* multiply current result by current base */
        base *= base;       /* square the base */
        e >>= 1;            /* shift exponent right by one */
    }

    return pow9 + (seq_t)n;  /* add n to get 9^n + n */
}
