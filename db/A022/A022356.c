/*
 * A022356  Fibonacci sequence beginning 0, 22.
 * https://oeis.org/A022356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*A000045(n) (Fibonacci with F(0)=0, F(1)=1)
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 927 (2026-11-01)
 * Helpers:   fibonacci
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t, if needed */
#include "../utils/fibonacci.h"

seq_t a(idx_t n)
{
    seq_t f = fibonacci(n);
    return f * ((seq_t)22);
}
