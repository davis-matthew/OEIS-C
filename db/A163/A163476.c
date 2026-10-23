/*
 * A163476  a(n) = 20*a(n-1) - 97*a(n-2) for n > 1; a(0) = 3, a(1) = 33.
 * https://oeis.org/A163476
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - 97*a(n-2), a(0) = 3, a(1) = 33
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases explicitly. */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)33;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)33;  /* a(1) */
    seq_t cur;

    /* Iterate from 2 up to n, computing the recurrence. */
    for (idx_t i = 2; i <= n; ++i) {
        cur = ((seq_t)20) * prev1 - ((seq_t)97) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
