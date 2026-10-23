/*
 * A195508  Number of iterations in a Draim factorization of 2n+1.
 * https://oeis.org/A195508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p-1)/2 where p is the smallest divisor >1 of 2n+1
 * Range:     u8 n <= 259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 2n+1 is always odd and ≥3 for n≥1. */
    idx_t m = 2*n + 1;
    idx_t d = 0;

    /* Quick check for divisor 3. */
    if (m % 3 == 0) {
        d = 3;
    } else {
        /* Trial division using the 6k±1 optimization. */
        for (idx_t i = 5; i*i <= m; i += 6) {
            if (m % i == 0) { d = i; break; }
            if (m % (i + 2) == 0) { d = i + 2; break; }
        }
    }

    /* If no proper divisor was found, m itself is prime. */
    if (d == 0) d = m;

    return ((seq_t)d - 1) / 2;
}
