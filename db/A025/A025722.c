/*
 * A025722  Index of 7^n within the sequence of the numbers of the form
 *          4^i*7^j.
 * https://oeis.org/A025722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + Sum_{k=0..n} ceil(k * c) with c = log_4(7)
 * Range:     u8 n <= 18, u16 n <= 304, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* c = log_4(7) = ln(7) / ln(4) */
    static const double C = 1.4036774610288020;   /* high precision value */

    seq_t res = 1;              /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k) {
        double prod = (double)k * C; /* k * log_4(7) */
        idx_t floor_k = (idx_t)prod; /* floor(k * c) */
        res += (seq_t)floor_k + 1;   /* ceil(k * c) = floor_k + 1 (c is irrational) */
    }
    return res;
}
