/*
 * A188512  Lengths of successive runs of identical terms in A188511.
 * https://oeis.org/A188512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic pattern of run lengths: [2,1,2,1,2,1,1] repeating with period 7 */

static const seq_t pattern[7] = {2, 1, 2, 1, 2, 1, 1};

seq_t a(idx_t n)
{
    /* Indexing starts at 1. If n is 0, return 0 as undefined. */
    if (n == 0)
        return 0;
    return pattern[(n - 1) % 7];
}
