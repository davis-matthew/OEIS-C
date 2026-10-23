/*
 * A104934  Expansion of (1-x)/(1 - 3*x - 2*x^2).
 * https://oeis.org/A104934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2), with a(0)=1, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL) return (seq_t)1;
    if (n == 1ULL) return (seq_t)2;
    seq_t prev = (seq_t)1;
    seq_t curr = (seq_t)2;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = 3 * curr + 2 * prev;
        prev = curr;
        curr = next;
    }
    return curr;
}
