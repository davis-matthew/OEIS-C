/*
 * A161728  a(n) =
 *          ((3+sqrt(3))*(4+sqrt(3))^n-(3-sqrt(3))*(4-sqrt(3))^n)/sqrt(12).
 * https://oeis.org/A161728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 13*a(n-2), a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)7;

    seq_t prev = (seq_t)1;  /* a(0) */
    seq_t curr = (seq_t)7;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = (seq_t)8 * curr - (seq_t)13 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
