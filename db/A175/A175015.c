/*
 * A175015  y-values in the solution to x^2-57*y^2=1.
 * https://oeis.org/A175015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 302*a(n-1) - a(n-2); a(1)=0, a(2)=20
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)20;

    seq_t prev = (seq_t)0;
    seq_t curr = (seq_t)20;
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = curr * (seq_t)302 - prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
