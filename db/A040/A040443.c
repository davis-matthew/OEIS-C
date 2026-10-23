/*
 * A040443  Continued fraction for sqrt(465).
 * https://oeis.org/A040443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(n) = n‑th partial quotient in the continued fraction expansion
 *            of √465
 * Range:     u8 n <= 1, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction for sqrt(465) is periodic; the algorithm below generates
   the partial quotients without any redundant storage. */
seq_t a(idx_t n)
{
    const seq_t N  = (seq_t)465;
    const seq_t a0 = (seq_t)21;      /* floor(sqrt(465)) */
    if (n == 0) return a0;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

    for (idx_t i = 1; i <= n; ++i) {
        m = d * a - m;
        d = (N - m * m) / d;
        a = (a0 + m) / d;
    }
    return a;
}
