/*
 * A094795  a(n) = (1/n!)*A023043(n).
 * https://oeis.org/A094795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 + 15*n^5 + 100*n^4 + 355*n^3 + 694*n^2 + 689*n + 265
 *            Implemented via Horner's rule for efficiency and exactness
 * Range:     u8 none, u16 n <= 3, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)1;   /* initial constant for Horner */
    val = val * nn + (seq_t)15;
    val = val * nn + (seq_t)100;
    val = val * nn + (seq_t)355;
    val = val * nn + (seq_t)694;
    val = val * nn + (seq_t)689;
    val = val * nn + (seq_t)265;
    return val;
}
