/*
 * A003993  Sequence b_3 (n) arising from homology of partitions with even
 *          number of blocks.
 * https://oeis.org/A003993
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*3^(n-1) - 2*n
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p = 1;
    idx_t i;
    for (i = 1; i < n; i++)
        p *= 3;
    return (seq_t)2 * p - (seq_t)2 * (seq_t)n;
}
