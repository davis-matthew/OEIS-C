/*
 * A159808  Numerator of Hermite(n, 5/22).
 * https://oeis.org/A159808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-1) - 242*(n-1)*a(n-2), a(0)=1, a(1)=5 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)5;   /* a(1) */
    seq_t cur = 0;
    for (idx_t k = 2; k <= n; ++k) {
        seq_t mult = 242 * (seq_t)(k - 1);
        cur = 5 * a_prev1 - mult * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
