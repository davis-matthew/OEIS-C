/*
 * A173284  Triangle by columns, positive Fibonacci numbers in every column
 *          shifted down twice, for k > 0.
 * https://oeis.org/A173284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Fib(r - 2*c + 1) where (r,c) are the row and column
 *            indices of n in the triangular array T(r,c) = Fib(r-2*c+1), read
 *            in row‑major order
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 71 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    idx_t cum = 0;
    idx_t r   = 0;
    for (;;) {
        idx_t len = (r >> 1) + 1;          /* floor(r/2)+1 entries in row r */
        if (cum + len > n) break;
        cum += len;
        ++r;
    }
    idx_t k        = n - cum;              /* column number */
    idx_t fib_idx  = r - (k << 1) + 1;     /* r - 2*k + 1 >= 1 */
    return fib(fib_idx);
}
