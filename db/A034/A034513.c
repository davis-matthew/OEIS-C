/*
 * A034513  a(n) = 1^n + 3^n + 9^n.
 * https://oeis.org/A034513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 3^n + 9^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 185 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* compute 1 + 3^n + 9^n */
    seq_t result = (seq_t)1;
    seq_t pow3 = ipow((seq_t)3, (seq_t)n);
    seq_t pow9 = ipow((seq_t)9, (seq_t)n);
    result += pow3;
    result += pow9;
    return result;
}
