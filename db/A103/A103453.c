/*
 * A103453  a(n) = 0^n + 3^n - 1.
 * https://oeis.org/A103453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0^n + 3^n - 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;            /* 0^0 = 1, 3^0 = 1, so 1 + 1 – 1 = 1 */
    return pow3(n) - 1;      /* for n ≥ 1, 0^n = 0, so a(n) = 3^n – 1 */
}
