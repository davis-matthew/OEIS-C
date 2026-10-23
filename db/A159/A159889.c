/*
 * A159889  Numerator of Hermite(n, 16/23).
 * https://oeis.org/A159889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159889: a(n) = numerator(Hermite(n, 16/23) * 23^n)
   Recurrence:  a(0) = 1,  a(1) = 32
                a(n) = 32 * a(n-1) - 1058 * (n-1) * a(n-2)   for n ≥ 2
   (Derived from the physicists Hermite polynomial recurrence with x=16/23.)
*/
seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 32;

    seq_t prev2 = 1;      /* a(n-2) */
    seq_t prev1 = 32;     /* a(n-1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* curr = 32 * prev1 - 1058 * (i-1) * prev2 */
        curr = 32 * prev1;
        curr -= (seq_t)1058 * (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
