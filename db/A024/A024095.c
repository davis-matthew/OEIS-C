/*
 * A024095  a(n) = 8^n - n^7.
 * https://oeis.org/A024095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8^n - n^7
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 10, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 8^n via exponentiation by squaring, avoiding overflow on small types */
    seq_t pow_base = 8;            /* current power of 8 */
    seq_t pow_res  = 1;            /* result accumulator */
    idx_t exp = n;
    while (exp) {
        if (exp & 1) pow_res *= pow_base;
        exp >>= 1;
        if (exp) pow_base *= pow_base;  /* square only if more bits left */
    }

    /* n^7 via simple multiplication (exponent is a constant) */
    seq_t nseq = (seq_t)n;
    seq_t npow7 = 1;
    for (int i = 0; i < 7; ++i) npow7 *= nseq;

    return pow_res - npow7;
}
