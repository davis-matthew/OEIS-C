/*
 * A033653  Trajectory of 15 under map x->x + (x-with-digits-reversed).
 * https://oeis.org/A033653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 15, a(n) = a(n‑1) + reverse_decimal(a(n‑1))
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 83,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 978 (2026-11-01)
 * Helpers:   reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)15;
    seq_t prev = (seq_t)15;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t rev = reverse_digits(prev);
        prev = prev + rev;
    }
    return prev;
}
