/*
 * A178804  When dealing cards into 3 piles (Left, Center, Right), the number
 *          of cards in the n-th card's pile, if dealing in a pattern L, C, R,
 *          C, L, C, R, C, L, C, ... [as any thoughtful six-year-old will try
 *          to do when sharing a pile of candy among 3 people].
 * https://oeis.org/A178804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n/2 if n even; a(n) = ceil(n/4) if n odd
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two cases directly.  All operations are on
       integer types; no floating point or special libraries. */
    if ((n & 1) == 0)               /* even n */
        return (seq_t)(n / 2);

    /* odd n : ceil(n/4) = (n+3)/4 for integer arithmetic */
    return (seq_t)((n + 3) / 4);
}
