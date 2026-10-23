/*
 * A189173  Integers m such that m^3 is the sum of squares of m consecutive
 *          integers.
 * https://oeis.org/A189173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 46*a(n-1) - a(n-2) for n > 3 * a(1) = 0, a(2) = 1, a(3)
 *            = 47 * (OEIS A189173 – Integers m such that m^3 is the sum of
 *            squares * of m consecutive integers.)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first three terms directly. */
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 47;

    /* Iteratively compute using the recurrence. */
    seq_t prev2 = 1;   /* a(2) */
    seq_t prev1 = 47;  /* a(3) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = ((seq_t)46) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
