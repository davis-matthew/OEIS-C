/*
 * A110243  Row sums of a Jacobi triangle.
 * https://oeis.org/A110243
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = sum_{k=0..n} Jacobi(n, 2n-2k+1)
 * Range:     i8 n <= 64, i16 n <= 99, i32 n <= 99, i64 n <= 99, i128 n <= 99,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = sum_{m odd, 1 ≤ m ≤ 2n+1} Jacobi(n,m) */

#include <stdint.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = 2 * n + 1;
    for (idx_t m = 1; m <= limit; m += 2) {
        sum += jacobi_symbol((seq_t)n, (seq_t)m);
    }
    return sum;
}
