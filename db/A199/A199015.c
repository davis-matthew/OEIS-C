/*
 * A199015  G.f.: 1/(1-x) * Product_{n>=1} (1 - x^(2*n))^2/(1 - x^(2*n-1))^2.
 * https://oeis.org/A199015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Sum_{d | (4*k+1)} (-1)^floor(d/2) This is
 *            the partial sum of A008441(k), the number of ways of writing k
 *            as the sum of two triangular numbers
 * Range:     u8 n <= 162, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t k = 0; k <= n; ++k) {
        idx_t m = 4 * k + 1;            /* 4*k+1 is odd, fits in idx_t */
        int64_t sumk = 0;               /* signed intermediate, values are small */
        for (idx_t d = 1; d <= m / d; ++d) {  /* iterate over divisors without overflow */
            if (m % d == 0) {
                /* divisor d */
                int even = (((d >> 1) & 1) == 0);   /* floor(d/2) even? */
                sumk += even ? 1 : -1;

                /* paired divisor q = m/d */
                if (d != m / d) {
                    idx_t q = m / d;
                    int evenq = (((q >> 1) & 1) == 0);
                    sumk += evenq ? 1 : -1;
                }
            }
        }
        total += (seq_t)sumk;   /* sumk is always non‑negative for each k */
    }
    return total;
}
