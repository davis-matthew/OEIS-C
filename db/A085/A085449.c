/*
 * A085449  Horadam sequence (0,1,4,2).
 * https://oeis.org/A085449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2); Initial values: a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 39, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = (seq_t)0; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t an;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * (seq_t)2 + a0 * (seq_t)4;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
