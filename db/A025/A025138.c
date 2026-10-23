/*
 * A025138  a(n) = (1/2)*(3rd elementary symmetric function of C(n,0), C(n,1),
 *          ..., C(n,[ n/2 ])).
 * https://oeis.org/A025138
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/12) * ( S1³ – 3·S1·S2 + 2·S3 ) where S1 =
 *            ∑_{k=0}^{⌊n/2⌋} C(n,k) S2 = ∑_{k=0}^{⌊n/2⌋} C(n,k)² S3 =
 *            ∑_{k=0}^{⌊n/2⌋} C(n,k)³ This equals ½ times the third elementary
 *            symmetric function of C(n,0),C(n,1),…,C(n,⌊n/2⌋)
 * Range:     u8 none, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) /* any value is acceptable for indices below the offset */
        return (seq_t)0;

    idx_t m = n / 2;          /* floor(n/2) */
    seq_t S1 = 0, S2 = 0, S3 = 0;
    seq_t C = 1;              /* C(n,0) */

    /* include k = 0 */
    S1 += C;
    S2 += C * C;
    S3 += C * C * C;

    for (idx_t k = 1; k <= m; ++k) {
        /* C = C(n,k) computed from C(n,k-1) */
        C = C * ((seq_t)(n - k + 1)) / (seq_t)k;
        S1 += C;
        seq_t C2 = C * C;
        S2 += C2;
        S3 += C2 * C;        /* C³ */
    }

    /* a(n) = ½ * E3 = (S1³ – 3S1S2 + 2S3)/12 */
    seq_t term = (S1 * S1 * S1 - (seq_t)3 * S1 * S2 + (seq_t)2 * S3) / (seq_t)12;
    return term;
}
