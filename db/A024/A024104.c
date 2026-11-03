/*
 * A024104  a(n) = 9^n - n^3.
 * https://oeis.org/A024104
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n - n^3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 387 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* 9^n */
    seq_t pow9 = ipow((seq_t)9, (seq_t)n);

    /* n^3 */
    seq_t n_val = (seq_t)n;
    seq_t cube = n_val * n_val * n_val;

    return pow9 - cube;
}
