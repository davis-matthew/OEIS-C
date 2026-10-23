/*
 * A111895  Number of numbers m <= n such that 5 equals the second digit after
 *          decimal point of square root of n in decimal representation.
 * https://oeis.org/A111895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |{ m ≤ n : floor(100*sqrt(m)) mod 10 = 5 }|
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t q = 0;

    for (;; ++q) {
        /* t = 10*q + 5  (the lower endpoint of 10*q+5..10*q+6 in 100*sqrt(m)) */
        uint64_t t = 10*q + 5;

        /* A = t²  ( = (10*q+5)² ), B = (t+1)² = (10*q+6)²  */
        unsigned __int128 A = (unsigned __int128)t * t;
        unsigned __int128 B = (unsigned __int128)(t + 1) * (t + 1);

        /* minimal m for this q: ceil(A / 10000) */
        uint64_t m_min = (uint64_t)((A + 9999) / 10000);

        /* maximal m for this q: floor((B-1) / 10000)   */
        uint64_t m_max = (uint64_t)((B - 1) / 10000);

        /* no further q can contribute */
        if (m_min > n)
            break;

        if (m_max > n)
            m_max = n;

        if (m_max >= m_min)
            count += (seq_t)(m_max - m_min + 1);
    }

    return count;
}
