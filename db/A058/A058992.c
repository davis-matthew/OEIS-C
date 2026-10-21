/*
 * A058992  Gossip Problem: there are n people and each of them knows some
 *          item of gossip not known to the others. They communicate by
 *          telephone and whenever one person calls another, they tell each
 *          other all that they know at that time. How many calls are required
 *          before each gossip knows everything?
 * https://oeis.org/A058992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2*n - 4 for n >= 4 (with a(1)=0, a(2)=1, a(3)=3)
 * Range:     u8 n <= 129, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;
    return (seq_t)2 * (seq_t)n - (seq_t)4;
}
