/*
 * A163383  a(n) = (n-1)*2^n - 1.
 * https://oeis.org/A163383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (n-1)*2^n - 1
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 26, i64 n <= 57, i128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t pow2_seq(idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        res *= (seq_t)2;
    return res;
}

seq_t a(idx_t n)
{
    seq_t two_n = pow2_seq(n);
    seq_t mult  = (seq_t)(n - (idx_t)1);
    return mult * two_n - (seq_t)1;
}
