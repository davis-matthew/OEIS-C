/*
 * A174777  y-values in the solution to x^2 - 38*y^2 = 1.
 * https://oeis.org/A174777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 74*a(n-1) - a(n-2), a(1)=0, a(2)=6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)6;

    seq_t prev = (seq_t)0;
    seq_t curr = (seq_t)6;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)74 * curr - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
