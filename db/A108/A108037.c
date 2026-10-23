/*
 * A108037  Triangle read by rows: n-th row is n-th nonzero Fibonacci number
 *          repeated n+1 times.
 * https://oeis.org/A108037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(0)=0; for n>0 let r be the smallest integer with r(r+3)/2 ≥ n,
 *            then a(n) = F_r, the r‑th nonzero Fibonacci number
 * Range:     u8 n <= 104, u16 n <= 324, u32 n <= 1175, u64 n <= 4464,
 *            u128 n <= 7380, big any n
 * Helpers:   fib_nonzero, row_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/fib_nonzero.h"
#include "../utils/row_index.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;
    uint64_t r = row_index((uint64_t)n);
    return fib_nonzero(r);
}
