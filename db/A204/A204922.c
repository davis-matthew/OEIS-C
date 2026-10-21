/*
 * A204922  Ordered differences of Fibonacci numbers.
 * https://oeis.org/A204922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = F(row+2) – F(col+1) where row = smallest integer with
 *            row*(row+1)/2 ≥ n and col = n – (row-1)*row/2. It enumerates the
 *            triangular array T(row,col) = F(row+2)-F(col+1)
 * Range:     u8 n <= 66, u16 n <= 253, u32 n <= 1035, u64 n <= 4186,
 *            u128 n <= 5050, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* compute the row index */
    idx_t row = (idx_t)((sqrt((long double)(8.0L*n + 1)) - 1.0L) / 2.0L);
    if (row * (row + 1) / 2 < n) ++row;

    idx_t prev = (row - 1) * row / 2; // number of terms before this row
    idx_t col = n - prev;             // 1‑based column within the row

    seq_t fn2 = fib(row + 2);  /* Fibonacci(row+2) */
    seq_t fk1 = fib(col + 1);  /* Fibonacci(col+1) */
    return fn2 - fk1;
}
