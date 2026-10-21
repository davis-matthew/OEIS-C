/*
 * A215931  Product of Fibonacci and Catalan numbers: a(n) =
 *          A000045(2*n+2)*A000108(n).
 * https://oeis.org/A215931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n+2) * Catalan(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 40,
 *            big any n
 * Helpers:   catalan, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* compute Fibonacci(2*n+2) and Catalan(n), then take the product */
    seq_t fib_val = fib(2*n + 2);
    seq_t cat_val = catalan(n);
    return fib_val * cat_val;
}
