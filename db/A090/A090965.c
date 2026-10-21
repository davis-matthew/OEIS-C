/*
 * A090965  a(n) = 8*a(n-1) - 4*a(n-2), where a(0) = 1, a(1) = 4.
 * https://oeis.org/A090965
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 4*a(n-2) (initial values: a(0)=1, a(1)=4)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;

    seq_t a0 = (seq_t)1;          /* a(n-2) */
    seq_t a1 = (seq_t)4;          /* a(n-1) */
    seq_t an;                     /* a(n) */
    idx_t i;

    for (i = 2; i <= n; ++i) {
        an = (a1 << 3) - (a0 << 2); /* 8*a1 - 4*a0 */
        a0 = a1;
        a1 = an;
    }
    return a1;
}
