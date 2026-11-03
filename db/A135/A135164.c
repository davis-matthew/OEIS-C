/*
 * A135164  a(n) = 7^n - 5^n + 3^n + 2^n.
 * https://oeis.org/A135164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 5^n + 3^n + 2^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 973 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The seq_pow utility implements exponentiation for the build's seq_t
 * type (including arbitrary‑precision integers).  We call it once
 * for each of the four constant bases. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t p7 = ipow_idx((seq_t)7, n);
    seq_t p5 = ipow_idx((seq_t)5, n);
    seq_t p3 = ipow_idx((seq_t)3, n);
    seq_t p2 = ipow_idx((seq_t)2, n);
    return p7 - p5 + p3 + p2;
}
