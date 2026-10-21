/*
 * A072670  Number of ways to write n as i*j + i + j, 0 < i <= j.
 * https://oeis.org/A072670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((d(n+1) - 1) / 2) where d(m) is the number of
 *            divisors of m. This counts unordered factor pairs (i+1)(j+1)=n+1
 *            with i>0 and i≤j
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Work with unsigned 64‑bit arithmetic. */
    idx_t m = n + 1;                 /* m = n + 1 */
    seq_t tau = 0;                   /* divisor count of m */

    /* Count divisors by iterating up to sqrt(m).
       Use the inequality i ≤ m / i to avoid overflow. */
    for (idx_t i = 1; i <= m / i; ++i) {
        if (m % i == 0) {
            if (i == m / i) {       /* perfect square: count once */
                tau += 1;
            } else {                /* pair of distinct divisors */
                tau += 2;
            }
        }
    }

    /* Convert divisor count to the required sequence value. */
    return (tau > 0) ? (tau - 1) / 2 : 0;
}
