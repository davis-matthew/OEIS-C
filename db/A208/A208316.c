/*
 * A208316  Number of n X 2 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 6*a(n-2) for n>3; a(1) = 2, a(2) = 13, a(3) =
 *            113. Recurrence is used to compute the sequence term
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first few values directly. */
    if (n == 0) return 0;          /* Not defined for 0, return 0 to avoid UB */
    if (n == 1) return 2;
    if (n == 2) return 13;
    if (n == 3) return 113;

    /* Use the recurrence for n >= 4. */
    seq_t prev2 = 13;  /* a(2) */
    seq_t prev1 = 113; /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 8 * prev1 + 6 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
