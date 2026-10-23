/*
 * A159510  Numerator of Hermite(n, 9/14).
 * https://oeis.org/A159510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=9 a(n) = 9*a(n-1) - 98*(n-1)*a(n-2) (for n ≥ 2)
 *            This follows from the recurrence of Hermite polynomials at
 *            x=9/14 multiplied by 7ⁿ, yielding integer numerators
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;
    seq_t prev = (seq_t)1;      /* a(n-2) */
    seq_t curr = (seq_t)9;      /* a(n-1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t next = (seq_t)9 * curr
                   - prev * (seq_t)98 * (seq_t)(k - 1);
        prev = curr;
        curr = next;
    }
    return curr;
}
