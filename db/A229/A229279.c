/*
 * A229279  Number of ascending runs in {1,...,5}^n.
 * https://oeis.org/A229279
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(n-1)*(3*n+2) for n>0, a(0) = 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 25*a(n-2) for n > 2 */
/* Initial values: a(0) = 0, a(1) = 5, a(2) = 40 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 5;
    if (n == 2) return 40;

    seq_t prev2 = 5;          /* a(1) */
    seq_t prev1 = 40;         /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = ((seq_t)10 * prev1) - ((seq_t)25 * prev2);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
