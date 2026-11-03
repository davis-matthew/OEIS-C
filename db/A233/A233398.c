/*
 * A233398  n^n mod 3^n.
 * https://oeis.org/A233398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^n mod 3^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 34 (2026-11-01)
 * Helpers:   ipow_i32, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0; a(n) = 0 when n is a multiple of 3 */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n % 3 == 0) return 0;

    seq_t mod = ipow_i32((seq_t)3, (int)n);
    return powmod((seq_t)n, n, mod);
}
