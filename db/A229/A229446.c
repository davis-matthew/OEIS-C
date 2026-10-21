/*
 * A229446  Number of 3 X n 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 8*n)/3 + 5 (valid for n ≥ 1)
 * Range:     u8 n <= 8, u16 n <= 58, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the unused offset n = 0, though the OEIS offset is 1. */
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t res = (nn * nn * nn + 8 * nn) / 3 + 5;
    return res;
}
