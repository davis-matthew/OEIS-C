/*
 * A081607  Number of numbers <= n having at least one 0 in their ternary
 *          representation.
 * https://oeis.org/A081607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 316, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_no_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = (n+1) – C(n)  where C(n) counts the numbers ≤ n whose ternary
 *  representation contains no zero digit. The function uses a digit‑DP
 *  that works for any integer type seq_t and idx_t. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/count_no_zero.h"

seq_t a(idx_t n)
{
    seq_t noZero = count_no_zero(n);
    seq_t total  = (seq_t)n + (seq_t)1;
    return total - noZero;
}
