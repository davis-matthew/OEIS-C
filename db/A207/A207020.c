/*
 * A207020  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n^3 + 27*n^2/2 - 21*n/2
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t part1 = 6 * n3;
    seq_t part2 = (27 * n2) / 2;
    seq_t part3 = (21 * nn) / 2;
    return part1 + part2 - part3;
}
