/*
 * A008853  Numbers k such that k^2 and k have same last 3 digits.
 * https://oeis.org/A008853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(4n)=1000*n, a(4n+1)=1000*n+1, a(4n+2)=1000*n+376,
 *            a(4n+3)=1000*n+625
 * Range:     u8 n <= 2, u16 n <= 263, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* safeguard – not used for n≥1 */
        return (seq_t)0;

    idx_t m = n - 1;                     /* shift to zero‑based index */
    seq_t base = ((seq_t)(m / 4)) * (seq_t)1000;   /* 1000 * floor(m/4) */

    switch (m % 4) {
        case 0:
            return base;
        case 1:
            return base + (seq_t)1;
        case 2:
            return base + (seq_t)376;
        default: /* case 3 */
            return base + (seq_t)625;
    }
}
