/*
 * A157802  a(n) = 27225*n^2 - 51302*n + 24168.
 * https://oeis.org/A157802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 27225*n^2 - 51302*n + 24168
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 398, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term1 = (seq_t)27225 * nn * nn;
    seq_t term2 = (seq_t)51302 * nn;
    /* Unsigned wrap‑around yields the correct positive result for all n >= 1 */
    return term1 - term2 + (seq_t)24168;
}
