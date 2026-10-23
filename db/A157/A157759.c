/*
 * A157759  a(n) = 15780962*n^2 - 25943924*n + 10662963.
 * https://oeis.org/A157759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15780962*n^2 - 25943924*n + 10662963
 * Range:     u8 none, u16 none, u32 n <= 17, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to the correct type before arithmetic */
    seq_t nn = (seq_t)n;
    return (seq_t)15780962 * nn * nn
         - (seq_t)25943924 * nn
         + (seq_t)10662963;
}
