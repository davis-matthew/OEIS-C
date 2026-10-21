/*
 * A228536  y-values in the solution to the Pell equation x^2 - 53*y^2 = -1.
 * https://oeis.org/A228536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 132498*a(n-1) - a(n-2); a(1) = 25; a(2) = 3312425
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 1) return 25;
    if (n == 2) return 3312425;

    /* iterative application of the recurrence */
    seq_t prev2 = 25;          /* a(1) */
    seq_t prev1 = 3312425;     /* a(2) */
    seq_t cur   = 0;           /* placeholder for a(i) */

    for (idx_t i = 3; i <= n; ++i) {
        cur   = 132498 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    /* when n >= 3, prev1 holds a(n) */
    return prev1;
}
