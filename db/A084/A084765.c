/*
 * A084765  a(n) = 2*a(n-1)^2 - 1, a(0)=1, a(1)=5.
 * https://oeis.org/A084765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=5, a(n)=2*a(n-1)^2-1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    seq_t prev = (seq_t)5;
    for (idx_t i = 2; i <= n; ++i) {
        prev = (seq_t)2 * prev * prev - (seq_t)1;
    }
    return prev;
}
