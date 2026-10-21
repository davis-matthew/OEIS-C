/*
 * A051873  21-gonal numbers: a(n) = n*(19n - 17)/2.
 * https://oeis.org/A051873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(19*n - 17)/2
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type */
    seq_t nn = (seq_t)n;

    /* Compute (19*n - 17) */
    seq_t tmp = nn * (seq_t)19 - (seq_t)17;

    /* a(n) = n * tmp / 2 */
    seq_t result = nn * tmp / (seq_t)2;

    return result;
}
