/*
 * A159005  Numerator of Hermite(n, 5/7).
 * https://oeis.org/A159005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 7^n * Hermite(n, 5/7) Recurrence: a(0) = 1, a(1) = 10,
 *            a(n+1) = 10*a(n) - 98*n*a(n-1) for n >= 1
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 7, i64 n <= 13, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    seq_t prev2 = (seq_t)1;   // a(0)
    seq_t prev1 = (seq_t)10;  // a(1)
    seq_t cur;
    for (idx_t i = 1; i < n; ++i) {   // i represents current n in recurrence
        cur = ((seq_t)10 * prev1) - ((seq_t)98 * (seq_t)i * prev2);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
