/*
 * A081609  Number of numbers <= n having at least one 1 in their ternary
 *          representation.
 * https://oeis.org/A081609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) - f(n), where f(n) = # of integers 0 ≤ x ≤ n whose
 *            ternary representation uses only the digits 0 and 2
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   count_no1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/count_no1.h"

seq_t a(idx_t n)
{
    /* Count all numbers 0..n. */
    seq_t total = (seq_t)n + (seq_t)1;

    /* Subtract those without a digit 1 in base 3. */
    seq_t without_one = count_no1(n);
    return total - without_one;
}
