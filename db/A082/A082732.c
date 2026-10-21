/*
 * A082732  a(1) = 1, a(2) = 3, a(n) = LCM of all the previous terms + 1.
 * https://oeis.org/A082732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3, a(3)=4, a(n)=a(n-1)^2-a(n-1)+1 for n>=4
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)3;
    if (n == (idx_t)3) return (seq_t)4;

    seq_t cur = (seq_t)4;          /* a(3) */
    for (idx_t i = (idx_t)4; i <= n; ++i) {
        cur = cur * cur - cur + (seq_t)1;
    }
    return cur;
}
