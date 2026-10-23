/*
 * A154022  a(n) = 5*A097780(n-2).
 * https://oeis.org/A154022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*a(n-1) - a(n-2), a(1)=0, a(2)=5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)5;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)5;   /* a(n-1) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)25 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
