/*
 * A155671  a(n) = 10^n - 9^n + 1^n.
 * https://oeis.org/A155671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 9^n + 1^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow10 = ipow((seq_t)10, (seq_t)n);
    seq_t pow9  = ipow((seq_t)9,  (seq_t)n);
    seq_t res   = pow10 - pow9 + (seq_t)1;
    return res;
}
