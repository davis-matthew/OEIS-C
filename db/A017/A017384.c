/*
 * A017384  a(n) = (10*n + 9)^8.
 * https://oeis.org/A017384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 9)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 24, u128 n <= 6552,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute base = 10*n + 9, in seq_t arithmetic to avoid overflow. */
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)9;
    /* Raise base to the 8th power by simple repeated multiplication. */
    seq_t result = (seq_t)1;
    for (int i = 0; i < 8; ++i) {
        result *= base;
    }
    return result;
}
