/*
 * A160330  Numerator of Hermite(n, 22/31).
 * https://oeis.org/A160330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=44,
   a(n)=44*a(n-1)-2*(n-1)*961*a(n-2)  for n≥2
   (Numerator of physicists' Hermite polynomial H_n(22/31)) */
seq_t a(idx_t n)
{
    seq_t a0 = 1;
    if (n == 0) return a0;
    seq_t a1 = 44;
    if (n == 1) return a1;

    seq_t prev2 = a0;
    seq_t prev1 = a1;
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1 = (seq_t)44 * prev1;
        seq_t term2 = (seq_t)2 * (seq_t)(i - 1) * (seq_t)961 * prev2;
        curr = term1 - term2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
