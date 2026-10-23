/*
 * A167821  a(n) is the number of n-tosses having a run of 3 or more heads or
 *          a run of 3 or more tails for a fair coin (i.e., probability is
 *          a(n)/2^n).
 * https://oeis.org/A167821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - 2*Fibonacci(n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   _fib_n_plus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_fib_n_plus1.h"

seq_t a(idx_t n) {
    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 <<= 1;
    }

    /* compute Fibonacci(n+1) */
    seq_t fib = _fib_n_plus1(n + 1);

    /* result = 2^n - 2*Fib(n+1) */
    seq_t two_fib = fib + fib;
    seq_t res = pow2 - two_fib;

    return res;
}
