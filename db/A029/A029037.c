/*
 * A029037  Expansion of 1/((1-x)(1-x^3)(1-x^4)(1-x^11)).
 * https://oeis.org/A029037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts {1, 3, 4, 11}
 *            Equivalent to the coefficient of 1/((1-x)(1-x^3)(1-x^4)(1-x^11))
 * Range:     u8 n <= 49, u16 n <= 363, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t d = 0; d <= n / 11; ++d)           /* number of 11‑parts */
    {
        idx_t rem_d = n - 11 * d;                 /* remaining sum */
        for (idx_t c = 0; c <= rem_d / 4; ++c)   /* number of 4‑parts   */
        {
            idx_t rem_dc = rem_d - 4 * c;        /* remaining after 4‑parts */
            total += (seq_t)(rem_dc / 3 + 1);    /* number of 3‑parts and 1‑parts */
        }
    }
    return total;
}
