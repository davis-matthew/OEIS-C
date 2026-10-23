/*
 * A174015  A generalized Catalan number sequence.
 * https://oeis.org/A174015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (3*n*a(n-1) - 3*(n-1)*a(n-2) - 3*(n-4)*a(n-3) -
 *            2*(7-2*n)*a(n-4)) / (n+1) with initial values a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=3, a(4)=4
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 33, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 3;
    if (n == 4) return 4;

    /* We need the four previous terms to compute the next one. */
    seq_t prev4 = 1; /* a(n-4) */
    seq_t prev3 = 1; /* a(n-3) */
    seq_t prev2 = 2; /* a(n-2) */
    seq_t prev1 = 3; /* a(n-1) */
    seq_t curr  = 0;

    for (idx_t i = 4; i <= n; ++i) {
        /* Compute a(i) from the recurrence. */
        seq_t numerator = (seq_t)3 * (seq_t)i * prev1
                         - (seq_t)3 * (seq_t)(i - 1) * prev2
                         - (seq_t)3 * (seq_t)(i - 4) * prev3
                         - (seq_t)2 * ((seq_t)7 - (seq_t)2 * (seq_t)i) * prev4;
        curr = numerator / ((seq_t)(i + 1));

        /* Shift the window of previous terms. */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1; /* a(n) */
}
