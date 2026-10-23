/*
 * A147610  a(n) = 3^(wt(n-1)-1), where wt() = A000120().
 * https://oeis.org/A147610
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(popcount(n-1)-1) (OEIS A147610)
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 2)           /* undefined below the offset – return a neutral value */
        return (seq_t)0;

    idx_t m = n - 1;     /* argument for popcount */
    idx_t pop = 0;
    while (m != 0) {
        pop += m & 1;
        m >>= 1;
    }

    /* popcount is at least 1 for n ≥ 2, so exponent is non‑negative */
    idx_t exp = pop - 1;
    return pow3(exp);
}
