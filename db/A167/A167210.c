/*
 * A167210  Tennis scores: values of successive points in a game.
 * https://oeis.org/A167210
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   a(n) = min(40, 15*n) (a(0)=0, a(1)=15, a(2)=30, a(n)=40 for n≥3)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return (seq_t)(15 * n);
    return (seq_t)40;
}
