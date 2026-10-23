/*
 * A020520  12th cyclotomic polynomial evaluated at powers of 2.
 * https://oeis.org/A020520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1-4^n+16^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute 4^n using exponentiation by squaring */
    seq_t pow4 = 1;
    seq_t base4 = 4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) pow4 *= base4;
        base4 *= base4;
        exp >>= 1;
    }

    /* compute 16^n using exponentiation by squaring */
    seq_t pow16 = 1;
    seq_t base16 = 16;
    exp = n;
    while (exp) {
        if (exp & 1) pow16 *= base16;
        base16 *= base16;
        exp >>= 1;
    }

    return 1 + pow16 - pow4;
}
