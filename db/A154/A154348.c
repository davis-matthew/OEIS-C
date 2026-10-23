/*
 * A154348  a(n) = 16*a(n-1) - 56*a(n-2) for n>1, with a(0)=1, a(1)=16.
 * https://oeis.org/A154348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 56*a(n-2) a(0) = 1, a(1) = 16 This
 *            implementation computes the term directly by iterating the
 *            recurrence; it works for any seq_t that supports the standard
 *            arithmetic operators
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 16;

    seq_t prev2 = 1;     /* a(0) */
    seq_t prev1 = 16;    /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 16 - prev2 * 56;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
