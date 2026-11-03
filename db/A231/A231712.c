/*
 * A231712  a(n) = n^n + n - 1.
 * https://oeis.org/A231712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n + n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 49 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute n^n */
    seq_t res = ipow((seq_t)n, (seq_t)n);

    /* add n and subtract 1 */
    res += (seq_t)n;
    res -= 1;

    return res;
}
