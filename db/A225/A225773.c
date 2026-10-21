/*
 * A225773  The squares on a chessboard that are black, counting from top left
 *          corner and down.
 * https://oeis.org/A225773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = 2*n - ((n-1)/4 mod 2)
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements sequence A225773 (black squares on a chessboard). */
seq_t a(idx_t n)
{
    /* Compute 2*n in the target sequence type. */
    seq_t val = (seq_t)n * 2;

    /* Determine whether the current block (of 4 terms) is odd or even.
       Blocks are numbered 0,1,2,… where each block contains 4 terms. */
    idx_t block = (n - 1) / 4;

    /* If the block index is odd, subtract 1 from the value. */
    if (block & 1)
        val -= 1;

    return val;
}
