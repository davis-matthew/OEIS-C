/*
 * A090242  a(n) = F(n)^F(n-1), where F is the Fibonacci sequence A000045.
 * https://oeis.org/A090242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n)^F(n-1), where F is the Fibonacci sequence A000045
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Helpers:   fib_pair, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses fast Fibonacci pairing and big‑integer exponentiation
   provided by the helper functions fib_pair and pow_seq_large. */

#include "../utils/fib_pair.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    /* fib_pair returns F(n) in fn and F(n+1) in fn1 */
    fib_pair(n, &fn, &fn1);

    /* F(n-1) = F(n+1) - F(n) */
    seq_t fn_minus1 = fn1 - fn;

    /* Compute fn ^ fn_minus1 */
    return pow_seq_large(fn, fn_minus1);
}
