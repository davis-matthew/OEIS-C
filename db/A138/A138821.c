/*
 * A138821  Concatenation of n-th Fibonacci number and n-th prime.
 * https://oeis.org/A138821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = concat( fib(n), prime(n) )
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 33, u64 n <= 79, u128 n <= 168,
 *            big any n
 * Helpers:   fib, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"        /* seq_t fib(idx_t n); */
#include "../utils/get_prime.h"  /* uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* Obtain the n‑th Fibonacci number and the n‑th prime. */
    seq_t fn = fib(n);                /* n‑th Fibonacci number */
    uint32_t pn = get_prime(n);       /* n‑th prime number */

    /* Count decimal digits of the prime. */
    int d = 0;
    uint32_t t = pn;
    do { ++d; t /= 10; } while (t);

    /* Compute 10^d in seq_t. */
    seq_t shift = 1;
    for (int i = 0; i < d; ++i)
        shift *= (seq_t)10;

    /* Concatenate: (fib * 10^d) + prime. */
    return fn * shift + (seq_t)pn;
}
