/*
 * A080029  a(n) is taken to be the smallest positive integer not already
 *          present which is consistent with the condition "n is a member of
 *          the sequence if and only if a(n) is a multiple of 3".
 * https://oeis.org/A080029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(3m)=6m, a(3m+1)=3m+2, a(3m+2)=6m+3
 * Range:     u8 n <= 128, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n / 3;            /* floor division */
    seq_t m_seq = (seq_t)m;      /* convert to seq_t */
    switch (n % 3) {
        case 0:
            return 6 * m_seq;          /* a(3m) = 6m */
        case 1:
            return 3 * m_seq + (seq_t)2;  /* a(3m+1) = 3m+2 */
        case 2:
            return 6 * m_seq + (seq_t)3;  /* a(3m+2) = 6m+3 */
    }
    /* unreachable, but keep compiler happy */
    return (seq_t)0;
}
