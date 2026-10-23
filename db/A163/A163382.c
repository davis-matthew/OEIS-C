/*
 * A163382  a(n) = the (decimal equivalent of the) smallest integer that can
 *          be made by rotating the binary digits of n any number of positions
 *          to the left or right, where a(n) in binary must contain the same
 *          number of digits (without any leading 0's) as n written in binary.
 * https://oeis.org/A163382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the smallest integer that can be obtained by
   cyclically rotating the binary representation of n while preserving
   the length (i.e., only rotations that start with a 1 are allowed). */
seq_t a(idx_t n)
{
    /* n must be ≥1 in the OEIS sequence; return 0 for n==0 as a
       default value that will never be used. */
    if (n == 0) {
        return 0;
    }

    seq_t sn = (seq_t)n;

    /* Compute the number of bits d of n without relying on a
       helper function. */
    unsigned d = 0;
    seq_t t = sn;
    while (t) {
        ++d;
        t >>= 1;
    }

    /* Build a d‑bit mask of all 1s:  (1 << d) - 1 would overflow
       when d equals the word size.  A simple loop avoids UB. */
    seq_t mask = 0;
    for (unsigned k = 0; k < d; ++k) {
        mask = (mask << 1) | (seq_t)1;
    }

    seq_t best = sn;          /* the original value is one candidate */

    /* Test every non‑trivial rotation.  For i=0 we already considered
       the original value. */
    for (unsigned i = 1; i < d; ++i) {
        /* Rotate left by i bits within a d‑bit field. */
        seq_t rot = ((sn << i) | (sn >> (d - i))) & mask;

        /* A rotation is valid only if its most significant bit is 1. */
        if ((rot >> (d - 1)) & (seq_t)1) {
            if (rot < best) {
                best = rot;
            }
        }
    }

    return best;
}
