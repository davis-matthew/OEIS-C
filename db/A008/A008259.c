/*
 * A008259  Coordination sequence T2 for Moganite, also for BGB1.
 * https://oeis.org/A008259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3m) = 22*m*m + 2, a(3m+1) = 22*m*m + 15*m + 4, a(3m+2) =
 *            22*m*m + 29*m + 11, with a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    idx_t m = n / 3;
    idx_t r = n % 3;
    seq_t m_sq = (seq_t)m * m;
    switch (r) {
        case 0: return (seq_t)22 * m_sq + 2;
        case 1: return (seq_t)22 * m_sq + (seq_t)15 * m + 4;
        case 2: return (seq_t)22 * m_sq + (seq_t)29 * m + 11;
    }
    return 0; /* unreachable */
}
