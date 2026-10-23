/*
 * A046172  Indices of pentagonal numbers (A000326) that are also squares
 *          (A000290).
 * https://oeis.org/A046172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=81; a(n)=98*a(n-1)-a(n-2)-16 for n≥3 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* outside defined range */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)81;

    seq_t prev2 = (seq_t)1;          /* a(1)   */
    seq_t prev1 = (seq_t)81;        /* a(2)   */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)98 * prev1 - prev2 - (seq_t)16;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
