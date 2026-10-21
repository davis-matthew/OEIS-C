/*
 * A070668  Smallest m in range 2..n-1 such that m^3 == 1 mod n, or 1 if no
 *          such number exists.
 * https://oeis.org/A070668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 384, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: for given n, find the smallest m in 2..n-1 such that m^3 ≡ 1 (mod n).
   If no such m exists, return 1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 2)            /* a(1)=1, a(2)=1 */
        return (seq_t)1;

    seq_t ns = (seq_t)n;   /* modulo base as seq_t */

    for (idx_t m = 2; m < n; ++m) {
        seq_t m_seq = (seq_t)m;
        seq_t m2    = (m_seq * m_seq) % ns;   /* m^2 mod n */
        seq_t m3    = (m2   * m_seq) % ns;    /* m^3 mod n */
        if (m3 == (seq_t)1)
            return m_seq;                    /* found smallest m */
    }

    return (seq_t)1;   /* no solution in range */
}
