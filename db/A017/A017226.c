/*
 * A017226  a(n) = (9*n + 5)^6.
 * https://oeis.org/A017226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 180, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t before multiplying to avoid overflow. */
    seq_t m = (seq_t)n * 9 + 5;
    /* Compute the sixth power by repeated multiplication. */
    seq_t r = m;
    r *= m;
    r *= m;
    r *= m;
    r *= m;
    r *= m;   /* r == m^6 now */
    return r;
}
