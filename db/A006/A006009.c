/*
 * A006009  Number of paraffins.
 * https://oeis.org/A006009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*n*(n+2)^3 for n even; a(n) = (1/8)*(n+1)^3*(n+3)
 *            for n odd
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 429, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    
    if (n % 2 == 0) {
        seq_t k = sn / 2;
        seq_t k1 = k + 1;
        seq_t k1_cubed = k1 * k1 * k1;
        return 2 * k * k1_cubed;
    } else {
        seq_t m = (sn + 1) / 2;
        seq_t m1 = m + 1;
        seq_t m_cubed = m * m * m;
        return 2 * m_cubed * m1;
    }
}
