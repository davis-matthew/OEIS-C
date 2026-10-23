/*
 * A017119  a(n) = (8*n + 4)^7 = 4^7*(2*n + 1)^7.
 * https://oeis.org/A017119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 70, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (8*n + 4) once */
    seq_t x = ((seq_t)8 * (seq_t)n) + (seq_t)4;
    /* Raise x to the 7th power */
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < 7; ++i) {
        r *= x;
    }
    return r;
}
