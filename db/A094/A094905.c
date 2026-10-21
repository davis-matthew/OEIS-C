/*
 * A094905  Expansion of e.g.f.: exp(6*x)/(1-6*x)^(1/6).
 * https://oeis.org/A094905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n + 1)*a(n-1) - 36*(n-1)*a(n-2) with initial values
 *            a(0) = 1, a(1) = 7. This recurrence follows from the
 *            differential equation a(n) + (-6*n-1)*a(n-1) + 36*(n-1)*a(n-2) =
 *            0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          return (seq_t)1;
    if (n == 1)          return (seq_t)7;

    seq_t a_minus2 = (seq_t)1;
    seq_t a_minus1 = (seq_t)7;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = a_minus1 * (seq_t)(6*i + 1)
                  - a_minus2 * (seq_t)(36*(i-1));
        a_minus2 = a_minus1;
        a_minus1 = cur;
    }
    return a_minus1;
}
