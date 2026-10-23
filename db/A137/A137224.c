/*
 * A137224  Interleave 4*n^2, 1+4*n^2, 1+(2n+1)^2, (2n+1)^2 (or A016742,
 *          A053755, A069894, A016754).
 * https://oeis.org/A137224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   interleave 4*n^2, 1+4*n^2, 1+(2n+1)^2, (2n+1)^2 in a cycle of
 *            length 4
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t k = n / 4;          /* block index */
    idx_t r = n & 3;          /* position inside the block (0..3) */
    seq_t kk = (seq_t)k;      /* cast to seq_t for all arithmetic */

    if (r == 0) {
        return ((seq_t)4) * kk * kk;                     /* 4*k^2    */
    } else if (r == 1) {
        return ((seq_t)4) * kk * kk + ((seq_t)1);       /* 1+4*k^2  */
    } else if (r == 2) {
        seq_t t = ((seq_t)2) * kk + ((seq_t)1);         /* 2*k+1    */
        return t * t + ((seq_t)1);                     /* 1+(2k+1)^2 */
    } else { /* r == 3 */
        seq_t t = ((seq_t)2) * kk + ((seq_t)1);         /* 2*k+1    */
        return t * t;                                  /* (2k+1)^2  */
    }
}
