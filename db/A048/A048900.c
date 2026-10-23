/*
 * A048900  Heptagonal pentagonal numbers.
 * https://oeis.org/A048900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(2) = 4347 a(3) = 16701685 a(n) = 3842*a(n-1) - a(n-2)
 *            + 512 for n >= 4 (This recurrence is equivalent to the one given
 *            in OEIS.)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* not part of the sequence */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4347;
    if (n == 3) return (seq_t)16701685;

    seq_t prev2 = (seq_t)4347;          /* a(n-2) */
    seq_t prev1 = (seq_t)16701685;      /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)3842 * prev1 - prev2 + (seq_t)512;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
