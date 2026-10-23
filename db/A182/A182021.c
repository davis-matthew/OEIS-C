/*
 * A182021  Achromatic number of n-cycle.
 * https://oeis.org/A182021
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = m where s_m ≤ n < s_{m+1}, s_m = m^2/2 (m even) or
 *            m(m-1)/2 (m odd); if m is odd and n = s_m + 1 then a(n) = m−1.
 *            (This implements the definition given for OEIS A182021.)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* behavior for n < 3 is undefined by the OEIS offset */
    seq_t n_seq = (seq_t)n;       /* use seq_t every time we combine with seq_t values */

    idx_t m = 0;
    for (;;) {
        /* compute s_m */
        seq_t s_m = (m & 1) ?
                    ((seq_t)m * (m - 1) / 2) :
                    ((seq_t)m * m / 2);

        /* compute s_{m+1} */
        idx_t mp1 = m + 1;
        seq_t s_next = (mp1 & 1) ?
                       ((seq_t)mp1 * (mp1 - 1) / 2) :
                       ((seq_t)mp1 * mp1 / 2);

        if (n_seq < s_next) {
            if ((m & 1) && n_seq == s_m + 1) {
                return (seq_t)(m - 1);
            } else {
                return (seq_t)m;
            }
        }
        ++m;
    }
}
