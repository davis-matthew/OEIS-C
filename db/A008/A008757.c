/*
 * A008757  Expansion of (1+x^14)/((1-x)*(1-x^2)*(1-x^3)).
 * https://oeis.org/A008757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n^2 + 6*n + 12)/12) + floor(((n-14)^2 + 6*(n-14) +
 *            12)/12) for n >= 14 floor((n^2 + 6*n + 12)/12) for n < 14
 *            Equivalent to the coefficients of (1 +
 *            x^14)/((1-x)(1-x^2)(1-x^3))
 * Range:     u8 n <= 12, u16 n <= 342, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute f(n) = floor((n^2 + 6*n + 12) / 12) */
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn + (seq_t)6 * nn + (seq_t)12;
    val = val / (seq_t)12;

    if (n >= 14) {
        idx_t m = n - 14;
        seq_t mm = (seq_t)m;
        seq_t add = mm * mm + (seq_t)6 * mm + (seq_t)12;
        add = add / (seq_t)12;
        val += add;
    }

    return val;
}
