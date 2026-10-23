/*
 * A163071  a(n) = ((4+sqrt(5))*(3+sqrt(5))^n + (4-sqrt(5))*(3-sqrt(5))^n)/2.
 * https://oeis.org/A163071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 4*a(n-2); a(0) = 4, a(1) = 17
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    if (n == 1)
        return (seq_t)17;

    seq_t prev = (seq_t)4;
    seq_t curr = (seq_t)17;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)6 * curr - (seq_t)4 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
