/*
 * A169724  a(n) = (2*3^(n-1)+1)^2.
 * https://oeis.org/A169724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*3^(n-1)+1)^2
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 993 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* sequence defined for n≥1 */
    seq_t t = ipow((seq_t)3, (seq_t)(n - 1)); /* 3^(n-1) */
    t = t * (seq_t)2 + (seq_t)1;         /* 2*3^(n-1)+1 */
    return t * t;                       /* square the result */
}
