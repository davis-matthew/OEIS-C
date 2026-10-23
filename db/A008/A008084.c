/*
 * A008084  Coordination sequence T1 for Zeolite Code ACO, ASV, EDI, and THO.
 * https://oeis.org/A008084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4; For n > 1, a(n) = 2n^2 – 4n + 4 + p(n),
 *            where p(n) is 12‑periodic: {0, 0, 0, –1, –1, 1, 0, –2, 0, 1, –1,
 *            –1}
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 12‑periodic corrections to the quadratic part */
    static const seq_t C[12] = {
        (seq_t)2,  (seq_t)4,  (seq_t)9,  (seq_t)19,
        (seq_t)35, (seq_t)52, (seq_t)72, (seq_t)100,
        (seq_t)131, (seq_t)163, (seq_t)201, (seq_t)244
    };

    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)1;

    /* Compute m = floor(n/12) and k = n mod 12 */
    idx_t m_ = n / 12;
    idx_t k_ = n % 12;          /* same as: n - 12 * m_; */

    seq_t m = (seq_t)m_;
    seq_t k = (seq_t)k_;
    unsigned kint = (unsigned)k_;

    /* a(12*m + k) = 288*m^2 + 48*k*m + C[k] for all k < 12, m >= 0 */
    return 288 * m * m + 48 * k * m + C[kint];
}
