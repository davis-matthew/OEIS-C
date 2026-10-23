/*
 * A116768  Number of permutations of length n which avoid the patterns 1342,
 *          3214, 4213.
 * https://oeis.org/A116768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 5*Fibonacci(2*n - 1) - 7*2^(n - 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* not used for formula, but safe */
    seq_t nseq   = (seq_t)n;
    idx_t fib_arg = 2*n - 1;               /* 2*n - 1 */
    seq_t fib_val = fib(fib_arg);          /* Fibonacci(2*n - 1) */
    seq_t two_val = pow2(n-1);             /* 2^(n-1) */
    seq_t res  = 3*nseq + 5*fib_val - 7*two_val;
    return res;
}
