/*
 * A192333  Numbers that are "unsafe" when playing the game Dollar Nim, which
 *          is a Nim game where users can remove 1, 5, 10, or 25 cents from an
 *          initial pile of money. The most common version of the game is
 *          played with an initial amount of $1, hence the name.
 * https://oeis.org/A192333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*floor((n-1)/5) + 2*n - 2
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((n-1)/5) safely in idx_t, then promote to seq_t */
    seq_t term = 5 * (seq_t)((n - 1) / 5) + 2 * (seq_t)n - 2;
    return term;
}
