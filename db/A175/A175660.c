/*
 * A175660  Eight bishops and one elephant on a 3 X 3 chessboard. a(n) =
 *          2^(n+2) - 3*F(n+2).
 * https://oeis.org/A175660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+2) - 3*F(n+2) Initial values: a(0)=1, a(1)=2, a(2)=7
 *            (F is the Fibonacci sequence: F(0)=0, F(1)=1.)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* Compute 2^(n+2) */
    seq_t power = pow2((idx_t)n + 2U);

    /* Compute the (n+2)-th Fibonacci number */
    seq_t fib_val = fib((idx_t)n + 2U);

    /* Compute 3*F(n+2) and return the difference */
    return power - (fib_val * 3U);
}
