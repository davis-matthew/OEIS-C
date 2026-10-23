/*
 * A032525  Floor( 7*n^2/2 ).
 * https://oeis.org/A032525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(7*n*n/2)
 * Range:     u8 n <= 8, u16 n <= 136, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    if ((nn & 1) == 0) {
        seq_t k = nn >> 1;
        return (seq_t)14 * k * k;
    } else {
        seq_t k = nn >> 1;
        return (seq_t)14 * k * (k + 1) + 3;
    }
}
