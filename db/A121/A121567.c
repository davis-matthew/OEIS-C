/*
 * A121567  Fibonacci[ (p - 1) ], where p = Prime[n].
 * https://oeis.org/A121567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(Prime[n] - 1)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    /* For n < 1 the behavior is undefined; return 0 to be safe. */
    if (n == 0) return 0;
    uint32_t p = get_prime(n);          /* p = Prime[n]            */
    return fib((idx_t)p - (idx_t)1);  /* a(n) = Fibonacci(p-1)  */
}
