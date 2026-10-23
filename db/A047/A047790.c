/*
 * A047790  a(n) = Fibonacci(2*n)-2^n+1.
 * https://oeis.org/A047790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n) - 2^n + 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   big_pow, fib_pair_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/fib_pair_custom.h"

seq_t a(idx_t n)
{
    /* compute F_{2n} */
    seq_t f, fp1;
    fib_pair_custom(n << 1, &f, &fp1);

    /* compute 2^n */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* a(n) = F_{2n} - 2^n + 1 */
    return f - pow2 + 1;
}
