/*
 * A037822  Number of i such that d(i) > d(i-1), where Sum_{i=0..m} d(i)*7^i
 *          is the base-7 representation of n.
 * https://oeis.org/A037822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = #{ i : d(i) > d(i-1) } where n's base-7 digits d(i) are
 *            the coefficients in n = Σ_{i=0..m} d(i) * 7^i. That is, a(n)
 *            counts the positions where a digit increases when reading the
 *            base‑7 representation from the most significant side to the
 *            least
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    idx_t temp = n;
    if (temp == 0) return 0;          /* not needed for n>=1, but harmless      */
    seq_t count = 0;
    idx_t prev = temp % 7;            /* d(0) */
    temp /= 7;
    while (temp)                      /* remaining digits */
    {
        idx_t d = temp % 7;           /* next higher digit d(i)   */
        if (d > prev) count++;
        prev = d;
        temp /= 7;
    }
    return count;
}
