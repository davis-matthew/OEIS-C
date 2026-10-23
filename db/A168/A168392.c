/*
 * A168392  a(n) = 5 + 8*floor((n-1)/2).
 * https://oeis.org/A168392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 8*floor((n-1)/2)
 * Range:     u8 n <= 64, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor((n-1)/2) using unsigned arithmetic */
    seq_t t = (seq_t)(n - 1) / 2;
    return (seq_t)5 + (seq_t)8 * t;
}
