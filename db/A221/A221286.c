/*
 * A221286  Vsemirnov's sequence.
 * https://oeis.org/A221286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 106276436867, a(1) = 35256392432
 * Range:     u8 none, u16 none, u32 none, u64 n <= 41, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)106276436867ULL;
    if (n == 1)
        return (seq_t)35256392432ULL;

    seq_t a0 = (seq_t)106276436867ULL;
    seq_t a1 = (seq_t)35256392432ULL;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a0 + a1;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
