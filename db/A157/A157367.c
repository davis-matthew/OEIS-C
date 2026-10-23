/*
 * A157367  a(n) = 4802*n^2 + 196*n + 1.
 * https://oeis.org/A157367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4802*n^2 + 196*n + 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 945, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A157367: n offset = 1 */
seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;
    /* Compute 4802*n^2 + 196*n + 1 */
    seq_t val = (seq_t)4802 * nn * nn;
    val += (seq_t)196 * nn;
    val += (seq_t)1;
    return val;
}
