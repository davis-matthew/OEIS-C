/*
 * A036999  Restricted permutations.
 * https://oeis.org/A036999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * (F(n+2) + 1) where F(k) is the k‑th Fibonacci number
 *            with F(0)=0, F(1)=1
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib_pair_impl
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair_impl.h"

seq_t a(idx_t n)
{
    /* compute F(n+2) */
    idx_t m = n + 2;
    seq_t f, fp1;
    fib_pair_impl(m, &f, &fp1);

    seq_t result = f + (seq_t)1;
    result *= (seq_t)3;
    return result;
}
