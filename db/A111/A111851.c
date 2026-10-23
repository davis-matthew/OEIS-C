/*
 * A111851  Number of numbers m <= n such that 1 equals the first digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ m ≤ n | the first decimal digit of √m equals 1 }
 * Range:     u8 n <= 2611, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: for every integer k≥0, the numbers m with
 * sqrt(m) in [k+0.1, k+0.2) satisfy
 * m_low  = ceil((k+0.1)²) = k² + (20k+100)/100
 * m_high = floor((k+0.2)²) = k² + (10k+1)/25
 * Count all m in [m_low , m_high] that lie within [1 , n]. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence offset starts at 1 */
    seq_t count = 0;
    unsigned __int128 n128 = n;
    for (idx_t k = 0; ; ++k) {
        unsigned __int128 k128 = k;
        unsigned __int128 k2   = k128 * k128;

        unsigned __int128 m_low128  = k2 + (20 * k128 + 100) / 100;  /* ceil */
        if (m_low128 > n128) break;                               /* no further k */

        unsigned __int128 m_high128 = k2 + (10 * k128 + 1) / 25;    /* floor */

        unsigned __int128 low  = m_low128 > 1 ? m_low128 : 1;
        unsigned __int128 high = m_high128 < n128 ? m_high128 : n128;

        if (high >= low) {
            count += (seq_t)(high - low + 1);
        }
    }
    return count;
}
