/*
 * A049994  a(n) is the number of arithmetic progressions of 4 or more
 *          positive integers, nondecreasing with sum n.
 * https://oeis.org/A049994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 3166, u32 n <= 8404, u64 n <= 8404,
 *            u128 n <= 8404, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = #{ (a,d,k) | a>=1, d>=0, k>=4, 
 *                      n = k*a + d*k*(k-1)/2 } 
 * Count of arithmetic progressions of length ≥4, non‑decreasing,
 * of positive integers whose terms sum to n.
 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = n;               /* use seq_t for arithmetic */
    /* k is the length of the progression, must be at least 4 */
    for (idx_t k = 4; k <= n; ++k) {
        seq_t K2 = (seq_t)k * (seq_t)(k - 1) / 2;  /* k*(k-1)/2 */
        /* maximum d such that the sum stays positive:  n - d*K2 > 0  */
        seq_t dmax = 0;
        if (K2 != 0) dmax = (N - 1) / K2;
        /* iterate over all admissible differences d */
        for (idx_t d = 0; d <= (idx_t)dmax; ++d) {
            seq_t S = N - (seq_t)d * K2;     /* remaining sum after d terms */
            if (S < k) continue;             /* would give a < 1 */
            if (S % k == 0)                  /* a is integer */
                ++count;                     /* valid progression found */
        }
    }
    return count;
}
