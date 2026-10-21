/*
 * A078002  Expansion of (1-x)/(1-2*x+x^2+2*x^3).
 * https://oeis.org/A078002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of xⁿ in (1‑x)/(1‑2x+x²+2x³).
 *            Recurrence: a(0)=a(1)=a(2)=1, a(n) = 2*a(n‑1) – a(n‑2) –
 *            2*a(n‑3) for n ≥ 3
 * Range:     i8 n <= 9, i16 n <= 22, i32 n <= 42, i64 n <= 85, i128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1 || n == 2)
        return 1;

    seq_t a0 = 1; /* a(n‑3)   */
    seq_t a1 = 1; /* a(n‑2)   */
    seq_t a2 = 1; /* a(n‑1)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 2*a2 - a1 - 2*a0;   /* a(n) = 2*a(n‑1) – a(n‑2) – 2*a(n‑3) */
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return a2;
}
