/*
 * A160190  Prime terms multiplied by Fibonacci terms (omitting first two
 *          terms of Fibonacci sequence).
 * https://oeis.org/A160190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime_n * fib_{n+2}
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   fib_pair, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n < 1, but safe */
    seq_t fib_n2, fib_next;               /* fib_{n+2} and fib_{n+3} */
    fib_pair(n + 2, &fib_n2, &fib_next);  /* compute Fib_{n+2} */
    seq_t prime = (seq_t)get_prime(n);    /* n-th prime, 1‑indexed */
    return prime * fib_n2;                /* product in seq_t */
}
