/*
 * A024139  a(n) = 11^n - n^12.
 * https://oeis.org/A024139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 11^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 18, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 11^n */
    seq_t result_11_pow = 1;
    seq_t base_11 = (seq_t)11;
    idx_t exp = n;
    while (exp) {
        if (exp & 1) result_11_pow *= base_11;
        base_11 *= base_11;
        exp >>= 1;
    }

    /* n^12 */
    seq_t result_n_pow12 = 1;
    seq_t base_n = (seq_t)n;
    idx_t exp2 = 12;
    while (exp2) {
        if (exp2 & 1) result_n_pow12 *= base_n;
        base_n *= base_n;
        exp2 >>= 1;
    }

    return result_11_pow - result_n_pow12;
}
