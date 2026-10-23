/*
 * A040281  Continued fraction for sqrt(299).
 * https://oeis.org/A040281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the partial quotients of the simple continued fraction for sqrt(299).
   The algorithm follows the standard recurrence:
      m_k   = d_{k-1} * a_{k-1} - m_{k-1}
      d_k   = (N - m_k^2) / d_{k-1}
      a_k   = floor((a0 + m_k) / d_k)
   with N = 299, a0 = floor(sqrt(299)) = 17.  The period is 4: 3,2,3,34. */
seq_t a(idx_t n)
{
    const seq_t N = 299;
    const seq_t a0 = 17;

    if (n == 0) {
        return a0;
    }

    seq_t m = 0;      /* m_0 */
    seq_t d = 1;      /* d_0 */
    seq_t a_term = a0; /* a_0 */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute next triplet (m_k, d_k, a_k) */
        m = d * a_term - m;
        d = (N - m * m) / d;
        a_term = (a0 + m) / d;
    }

    return a_term;
}
