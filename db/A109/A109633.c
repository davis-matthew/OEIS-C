/*
 * A109633  In the game of bridge, a(n) is the penalty for going down n tricks
 *          in a non-vulnerable, doubled contract.
 * https://oeis.org/A109633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(1)=100, a(2)=300, a(3)=500, a(n)=300*n-400 for n>=4
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)100;
    if (n == 2) return (seq_t)300;
    if (n == 3) return (seq_t)500;
    return (seq_t)300 * n - (seq_t)400;
}
