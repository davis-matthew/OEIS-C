/*
 * A108051  a(n+1) = 4*(a(n)+a(n-1)) for n>1, a(1)=1, a(2)=6.
 * https://oeis.org/A108051
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=6; for n>2: a(n)=4*(a(n-1)+a(n-2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;

    seq_t prev_prev = 1; /* a(1) */
    seq_t prev      = 6; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t cur = 4 * (prev_prev + prev);
        prev_prev = prev;
        prev      = cur;
    }
    return prev;
}
