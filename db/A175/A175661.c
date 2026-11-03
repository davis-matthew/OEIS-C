/*
 * A175661  Eight bishops and one elephant on a 3 X 3 chessboard: a(n) =
 *          2^(n+2)-3*F(n+1), with F(n) = A000045(n).
 * https://oeis.org/A175661
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+2) - 3*F(n+1) (F = Fibonacci, A000045)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 934 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

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
    seq_t powv = pow2((idx_t)n + 2);
    seq_t fibv = fib((idx_t)n + 1);
    return powv - (seq_t)3 * fibv;
}
