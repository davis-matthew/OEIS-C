/*
 * A030426  a(n) = Fibonacci(prime(n)).
 * https://oeis.org/A030426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Fibonacci(prime(n))
 *
 * Computes the n-th term of A030426: the n-th Fibonacci number
 * whose index is the n-th prime. The function is defined for n ≥ 1.
 */

#include "../utils/get_prime.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* get_prime returns the n-th prime (1‑indexed).  */
    uint32_t p = get_prime(n);

    /* fib expects an idx_t argument: the prime index. */
    return fib((idx_t)p);
}
