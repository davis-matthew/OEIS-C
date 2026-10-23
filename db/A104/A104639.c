/*
 * A104639  Number of even digits in n^3.
 * https://oeis.org/A104639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = count_even_digits(n³)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 1769, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_even_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_even_digits.h"

/* Return the number of even decimal digits in n³. */
seq_t a(idx_t n)
{
    /* Compute n³ in seq_t to avoid overflow of idx_t.
       Multiplication is left-associative; use explicit
       casts to guarantee seq_t operands. */
    seq_t x = (seq_t)n;
    x = x * x * x;
    return count_even_digits(x);
}
