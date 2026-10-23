/*
 * A155828  Number of integers k in {1,2,3,..,n} such that kn+1 is a square.
 * https://oeis.org/A155828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{k ∈ {1,…,n} : k·n + 1 is a perfect square}|
 * Range:     u8 n <= 15, u16 n <= 261, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate over possible m with k = (m² – 1) / n and count those with 1 ≤ k ≤ n. */

#include <stdint.h>                     /* for UINT64_MAX if needed */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return 0;             /* safety, sequence starts at n = 1 */

    seq_t sn = (seq_t)n;               /* cast once for efficiency */

    /* Limit of m: m² – 1 ≤ n · n  ⇒  m ≤ √(n² + 1) < n + 1.
       We loop up to n + 1 (guarding against overflow) and check k ≤ n. */
    idx_t limit = n;
    if (limit != UINT64_MAX) limit = limit + 1;

    for (idx_t m = 2; m <= limit; ++m) {
        seq_t m_val = (seq_t)m;
        seq_t x = m_val * m_val - 1;   /* m² – 1 */
        if (x % sn == 0) {
            seq_t k = x / sn;          /* (m² – 1) / n */
            if (k <= sn) count++;
        }
    }
    return count;
}
