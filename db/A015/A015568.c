/*
 * A015568  Expansion of x/(1 - 7*x - 10*x^2).
 * https://oeis.org/A015568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 7*a(n-1) + 10*a(n-2) (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A015568.
   The sequence satisfies a linear recurrence of order 2.
   The implementation runs in O(n) time and uses O(1) extra space. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t prev2 = 0;          /* a(n-2) */
    seq_t prev1 = 1;          /* a(n-1) */
    seq_t curr  = 0;          /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)7 * prev1 + (seq_t)10 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
