/*
 * A035597  Number of points of L1 norm 3 in cubic lattice Z^n.
 * https://oeis.org/A035597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 2*n)/3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 1475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t t  = nn * nn;           /* n^2 */
    t = t * nn;                   /* n^3 */
    t = t * 4;                    /* 4*n^3 */
    t += nn * 2;                  /* + 2*n */
    t = t / 3;                    /* divide by 3 (exact division) */
    return t;
}
