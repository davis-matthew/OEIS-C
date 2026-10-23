/*
 * A040229  Continued fraction for sqrt(245).
 * https://oeis.org/A040229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) gives the n-th term of the continued fraction expansion of
 *            sqrt(245), with the first term a(0) = floor(sqrt(245)) = 15. The
 *            period of the continued fraction is 10: a(1..10) = [1, 1, 1, 7,
 *            6, 7, 1, 1, 1, 30]
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function works for any n >= 0, in O(1) time. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;
    /* 10-term periodic pattern after the initial term */
    static const seq_t pattern[10] = { (seq_t)1,  /* a(1) */
                                       (seq_t)1,  /* a(2) */
                                       (seq_t)1,  /* a(3) */
                                       (seq_t)7,  /* a(4) */
                                       (seq_t)6,  /* a(5) */
                                       (seq_t)7,  /* a(6) */
                                       (seq_t)1,  /* a(7) */
                                       (seq_t)1,  /* a(8) */
                                       (seq_t)1,  /* a(9) */
                                       (seq_t)30  /* a(10) */ };
    /* (n-1) is always positive here because n != 0 */
    idx_t idx = (n - (idx_t)1) % (idx_t)10;
    return pattern[idx];
}
