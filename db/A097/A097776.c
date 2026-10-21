/*
 * A097776  Pell equation solutions (14*b(n))^2 - 197*a(n)^2 = -1 with b(n) =
 *          A097775(n), n >= 0.
 * https://oeis.org/A097776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 785 a(n) = 786*a(n-1) - a(n-2) for n > 1 (OEIS
 *            A097776)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)785;

    /* Iteratively compute up to n */
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)785;    /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)786 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
