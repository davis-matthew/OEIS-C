/*
 * A100158  Structured disdyakis triacontahedral numbers (vertex structure
 *          11).
 * https://oeis.org/A100158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (110*n^3 - 150*n^2 + 46*n)/6
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 421, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    seq_t cu = sq * sn;
    seq_t term = (seq_t)110 * cu - (seq_t)150 * sq + (seq_t)46 * sn;
    return term / 6;
}
