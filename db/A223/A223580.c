/*
 * A223580  Number of nX3 0..3 arrays with antidiagonals unimodal.
 * https://oeis.org/A223580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=64, a(2)=4096, a(n)=50·a(n‑1) for n>2 (equivalently
 *            a(n)=4096·50^(n‑2) for n≥3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 185 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)64;
    if (n == 2) return (seq_t)4096;
    seq_t base = (seq_t)4096;
    seq_t exp  = (seq_t)(n - 2);
    seq_t pow  = ipow((seq_t)50, exp);
    return base * pow;
}
