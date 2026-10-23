/*
 * A186421  Even numbers interleaved with repeated odd numbers.
 * https://oeis.org/A186421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is even, else 2*floor(n/4)+1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the result type once */
    seq_t idx = (seq_t)n;
    /* return n itself if it is even */
    if ((idx & 1) == 0)
        return idx;
    /* otherwise compute 2*floor(n/4)+1 */
    return 2 * (idx / 4) + 1;
}
