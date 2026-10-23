/*
 * A104582  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product of the lower triangular matrix (Fibonacci(i-j+1)) and
 *          of the lower triangular matrix all of whose entries are equal to 1
 *          (for j <= i).
 * https://oeis.org/A104582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Fibonacci(row - col + 3) - 1 where (row,col) is the
 *            position of n in the triangle read by rows (row starts at 1, col
 *            starts at 1)
 * Range:     u8 n <= 66, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   fib_doubling, fib_idx, row_of_triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_doubling.h"
#include "../utils/fib_idx.h"
#include "../utils/row_of_triangle.h"

/* OEIS sequence A104582. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* out‑of‑range is harmless */

    idx_t row = row_of_triangle(n);
    unsigned __int128 before = ((unsigned __int128)row * (row - 1)) / 2;
    idx_t col = n - (idx_t)before;        /* 1‑based column in the row */
    idx_t m   = row - col + 3;            /* index for Fibonacci */

    seq_t f = fib_idx(m);
    return f - (seq_t)1;
}
