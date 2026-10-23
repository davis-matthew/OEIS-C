/*
 * A122769  Numbers k such that k^2 is of the form 3*m^2 + 2*m + 1 (A056109).
 * https://oeis.org/A122769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 11, a(n) = 14 * a(n-1) - a(n-2) (for n ≥ 3) –
 *            linear recurrence from OEIS A122769
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)11;

    /* iterate the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)11;  /* a(2) */
    seq_t curr;

    for (idx_t i = 3; i <= n; ++i) {
        curr = (seq_t)14 * prev1 - prev2;  /* a(i) = 14*a(i-1) - a(i-2) */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
