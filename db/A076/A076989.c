/*
 * A076989  Smallest cube of the form n*k + 1 with k>0.
 * https://oeis.org/A076989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 1631, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: for each n, find the smallest integer m >= 2 such that
   m³ ≡ 1 (mod n).  The returned value is m³.  For n = 1 the
   sequence definition uses k > 0, so the smallest cube is 2³ = 8,
   which is returned specially. */

seq_t a(idx_t n)
{
    /* Handle the exceptional case n = 1 */
    if (n == 1)
        return (seq_t)8;          /* 2³ */

    seq_t n_seq = (seq_t)n;

    for (idx_t m = 2; ; ++m)
    {
        seq_t m_seq = (seq_t)m;

        /* Efficiently compute (m³ mod n) */
        seq_t mod = (m_seq * m_seq) % n_seq;
        mod = (mod * m_seq) % n_seq;

        if (mod == (seq_t)1)
            return m_seq * m_seq * m_seq;  /* Return the cube */
    }
}
