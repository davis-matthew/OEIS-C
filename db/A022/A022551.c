/*
 * A022551  Numbers that are the sum of 2 squares and a nonnegative cube.
 * https://oeis.org/A022551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 943, u32 n <= 943, u64 n <= 943,
 *            u128 n <= 943, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A022551: a(n) is the n-th integer that can be written as
   a non‑negative cube plus a sum of two squares.  The function
   searches the integers in increasing order and tests each one
   directly. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* Check whether x can be expressed as a^2 + b^2 for integers a,b ≥ 0. */
static bool is_sum_of_two_squares(seq_t x)
{
    for (idx_t a = 0; (seq_t)a * a <= x; ++a) {
        seq_t rem = x - (seq_t)a * a;
        seq_t sq  = floor_sqrt(rem);
        if (sq * sq == rem) return true;
    }
    return false;
}

/* Return the n-th term (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* guard for n < 1 */
    idx_t count = 0;
    idx_t m = 0;

    while (count < n) {
        bool ok = false;
        /* try all cubes ≤ m */
        for (idx_t i = 0;; ++i) {
            seq_t cube = (seq_t)i * i * i;
            if (cube > (seq_t)m) break;
            seq_t rem = (seq_t)m - cube;
            if (is_sum_of_two_squares(rem)) { ok = true; break; }
        }
        if (ok) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        ++m;
    }
    return 0; /* unreachable */
}
