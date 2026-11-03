/*
 * A129371  a(n) = Sum_{k=0..floor(n/2)} (n-k)^2.
 * https://oeis.org/A129371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (n-k)^2 Equivalent: a(n) =
 *            Sum_{k=⌈n/2⌉}^n k^2 = S(n) – S(⌈n/2⌉-1)
 * Range:     u8 n <= 9, u16 n <= 60, u32 n <= 1634, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 56 terms through n = 4783 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Auxiliary function: S(x) = 1^2 + 2^2 + … + x^2 = x(x+1)(2x+1)/6 */
static seq_t square_sum(idx_t x) {
    seq_t X = (seq_t)x;
    return X * (X + 1) * (2 * X + 1) / 6;
}

seq_t a(idx_t n) {
    /* Handle the trivial case n = 0 explicitly */
    if (n == 0)
        return 0;

    /* ceil(n/2) = (n+1)/2 with integer division */
    idx_t m = (n + 1) / 2;

    seq_t sum_n = square_sum(n);
    seq_t sum_m_minus_1 = 0;
    if (m > 0)
        sum_m_minus_1 = square_sum(m - 1);

    return sum_n - sum_m_minus_1;
}
