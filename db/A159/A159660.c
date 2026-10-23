/*
 * A159660  Numerator of Hermite(n, 9/20).
 * https://oeis.org/A159660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1, a(1) = 9 Recurrence: a(n) = 9*a(n-1) -
 *            200*(n-1)*a(n-2) (n ≥ 2)
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the numerator of Hermite(n, 9/20) */
seq_t a(idx_t n)
{
    /* Trivial cases */
    if (n == 0) return 1;
    if (n == 1) return 9;

    /* Iterate the linear recurrence from 0 up to n */
    seq_t prev2 = 1;  /* a(0) */
    seq_t prev1 = 9;  /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i) {
        /* term = 9 * prev1 - 200 * (i-1) * prev2 */
        curr = 9 * prev1;
        curr = curr - (200 * (seq_t)(i - 1) * prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
