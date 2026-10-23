/*
 * A121461  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n, having last ascent of length k (1 <= k <=
 *          n).
 * https://oeis.org/A121461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For n ≥ 1, let r be the unique integer with r*(r-1)/2 < n ≤
 *            r*(r+1)/2 (the row number). Let c = n - r*(r-1)/2 (the column
 *            number within the row, 1‑based). Then the sequence value is a(n)
 *            = 1 if c == r (last element of the row) = Fib( 2*(r-c) )
 *            otherwise (Fib is the standard Fibonacci sequence wit...
 * Range:     u8 n <= 28, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;          // undefined below offset 1

    /* Find the row r such that r*(r-1)/2 < n ≤ r*(r+1)/2. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;

    /* First index of row r (zero‑based). */
    idx_t offset = r * (r - 1) / 2;

    /* Column index (1‑based). */
    idx_t c = n - offset;

    /* Return the triangle entry. */
    if (c == r) return 1;
    return fib( 2 * ( r - c ) );
}
