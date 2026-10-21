/*
 * A218745  a(n) = (42^n - 1)/41.
 * https://oeis.org/A218745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (42^n - 1) / 41
 * Recurrence: a(0) = 0, a(1) = 1,
 *              a(n) = 43 * a(n-1) - 42 * a(n-2)  for n ≥ 2
 *
 * This implementation uses only integer arithmetic and works for any
 * integer type seq_t supplied by the build (small to arbitrary precision).
 * It computes the value in O(n) time and O(1) additional space,
 * making it suitable for repeated calls with arbitrary n.
 */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    /* Iteratively compute using the recurrence relation. */
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t current = 0;

    for (idx_t i = 2; i <= n; ++i) {
        /* current = 43 * prev1 - 42 * prev2 */
        current = prev1 * (seq_t)43u - prev2 * (seq_t)42u;
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
