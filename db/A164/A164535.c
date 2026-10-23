/*
 * A164535  a(n) = 8*a(n-1) - 14*a(n-2) for n > 1; a(0) = 3, a(1) = 20.
 * https://oeis.org/A164535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 14*a(n-2) for n > 1; a(0) = 3, a(1) = 20
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)20;

    seq_t prev2 = (seq_t)3;  /* a(0) */
    seq_t prev1 = (seq_t)20; /* a(1) */
    seq_t current = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i) {
        current = 8 * prev1 - 14 * prev2;
        prev2 = prev1;
        prev1 = current;
    }

    return current;
}
