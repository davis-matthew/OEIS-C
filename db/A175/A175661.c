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
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t powv = pow2((idx_t)n + 2);
    seq_t fibv = fib((idx_t)n + 1);
    return powv - (seq_t)3 * fibv;
}
