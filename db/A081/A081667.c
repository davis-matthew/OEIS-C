/*
 * A081667  a(n) = Fibonacci(binomial(n+2,2)).
 * https://oeis.org/A081667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(binomial(n+2,2))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   fib_pair_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_pair_rec.h"

seq_t a(idx_t n)
{
    /* binomial(n+2,2) = (n+2)(n+1)/2 */
    idx_t k = ((unsigned __int128)(n + 2) * (n + 1)) / 2;

    seq_t fn, fn1;
    fib_pair_rec(k, &fn, &fn1);
    return fn;
}
