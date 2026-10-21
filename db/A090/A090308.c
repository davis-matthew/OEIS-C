/*
 * A090308  a(n) = 19*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 19.
 * https://oeis.org/A090308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 19*a(n-1) + a(n-2), a(0) = 2, a(1) = 19
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)19;

    seq_t a0 = (seq_t)2;
    seq_t a1 = (seq_t)19;
    seq_t ai = 0;

    for (idx_t i = 2; i <= n; ++i) {
        ai = a1 * (seq_t)19 + a0;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
