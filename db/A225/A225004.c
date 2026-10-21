/*
 * A225004  a(n) is the largest multiple of n with fewer than twice as many
 *          divisors as n.
 * https://oeis.org/A225004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest multiple of n with fewer than twice as many
 *            divisors as n
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function divisor_count_u64 to compute divisor numbers. */

#include <stdbool.h>
#include "../utils/divisor_count_u64.h"   // provides uint64_t divisor_count_u64(uint64_t)

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard – offset is 1 */

    /* divisor count of n */
    uint64_t d_n = divisor_count_u64((uint64_t)n);
    uint64_t limit_div = 2 * d_n;          /* the threshold */

    seq_t best = (seq_t)n;                 /* t=1 always satisfies the condition */

    /* search all multiples n*k, k = 1 .. n  (m <= n^2) */
    for (idx_t k = 2; k <= n; ++k) {
        uint64_t m_uint = (uint64_t)n * (uint64_t)k;
        /* compute divisor count of m */
        uint64_t d_m = divisor_count_u64(m_uint);
        if (d_m < limit_div) {
            seq_t m = (seq_t)m_uint;
            if (m > best) best = m;
        }
    }
    return best;
}
