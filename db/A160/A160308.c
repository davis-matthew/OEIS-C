/*
 * A160308  Numerator of Hermite(n, 10/31).
 * https://oeis.org/A160308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A160308
 *
 *  Definition (recurrence):
 *    a(0) = 1
 *    a(1) = 20
 *    a(n+2) = 20 * a(n+1) - 1922 * (n+1) * a(n)   for n >= 0
 *
 *  The function below computes a(n) iteratively in O(n) time
 *  without using any external libraries or dynamic allocation.
 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    seq_t a0 = (seq_t)1;            /* a(n-2) */
    seq_t a1 = (seq_t)20;           /* a(n-1) */
    seq_t a2;                       /* current term */

    for (idx_t i = 2; i <= n; ++i) {
        /* factor = 1922 * (i-1)   cast to seq_t to avoid overflow */
        seq_t factor = (seq_t)1922 * (seq_t)(i - 1);
        /* a2 = 20*a1 - factor*a0 */
        a2 = (seq_t)20 * a1 - factor * a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
