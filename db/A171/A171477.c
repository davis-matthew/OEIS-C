/*
 * A171477  a(n) = 6*a(n-1) - 8*a(n-2) + 1 for n > 1; a(0) = 1, a(1) = 7.
 * https://oeis.org/A171477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) + 1, with a(0)=1, a(1)=7
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)7;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)7;   /* a(1) */
    seq_t a_next;

    for (idx_t i = 2; i <= n; ++i) {
        a_next = a1 * 6 - a0 * 8 + 1;
        a0 = a1;
        a1 = a_next;
    }
    return a1;
}
