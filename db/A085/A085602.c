/*
 * A085602  Numbers of the form (2n+1)^(2n+1) + 1.
 * https://oeis.org/A085602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n-1)^(2*n-1)+1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 182 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Index n starts at 1; guarantee n >= 1 */
    seq_t base = (seq_t)(2 * (seq_t)n - 1);
    seq_t pow_val = ipow(base, base);   /* base^(base) */
    return pow_val + 1;
}
