/*
 * A090241  a(n) = F(n)^F(n+1), where F is the Fibonacci sequence A000045.
 * https://oeis.org/A090241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n)^F(n+1), where F is the Fibonacci sequence A000045
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   fib_pair, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n) {
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn  = F(n),  fn1 = F(n+1)   */
    return pow_seq_large(fn, fn1);    /* a(n) = F(n)^F(n+1)           */
}
