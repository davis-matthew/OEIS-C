/*
 * A090650  a(n) = n^(n+6).
 * https://oeis.org/A090650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes a(n) = n^(n+6) for n >= 1.
   Works for any seq_t type (small, 64‑bit, or arbitrary precision). */
seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = (seq_t)n;
    idx_t exp    = n + 6;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}
