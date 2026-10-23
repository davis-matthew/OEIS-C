/*
 * A163812  Expansion of (1 - x^5) * (1 - x^6) / ((1 - x) * (1 - x^10)) in
 *          powers of x.
 * https://oeis.org/A163812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n/10 + 1) - (n-5>=0 ? (n-5)/10 + 1 : 0) - (n-6>=0 ?
 *            (n-6)/10 + 1 : 0) + (n-11>=0 ? (n-11)/10 + 1 : 0) (for n ≥ 0)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)(n / 10 + 1);

    if (n >= 5)
        res -= (seq_t)(((n - 5) / 10) + 1);

    if (n >= 6)
        res -= (seq_t)(((n - 6) / 10) + 1);

    if (n >= 11)
        res += (seq_t)(((n - 11) / 10) + 1);

    return res;
}
