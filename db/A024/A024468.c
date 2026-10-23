/*
 * A024468  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [
 *          (n+1)/2 ], s = (Fibonacci numbers), t = (primes).
 * https://oeis.org/A024468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{⌊(n+1)/2⌋} Fib(i) * prime(n+1−i)
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* offset starts at 1 */

    const idx_t k = (n + 1) / 2;                /* floor((n+1)/2) */

    seq_t sum = (seq_t)0;
    seq_t f_prev = (seq_t)1;                    /* Fib(1)   */
    seq_t f_curr = (seq_t)1;                    /* Fib(2)   */
    seq_t f_i;

    for (idx_t i = 1; i <= k; ++i)
    {
        if (i == 1)
            f_i = f_prev;
        else if (i == 2)
            f_i = f_curr;
        else
        {
            seq_t f_next = f_prev + f_curr;     /* Fib(i)   */
            f_prev = f_curr;
            f_curr = f_next;
            f_i = f_next;
        }

        idx_t prime_index = n + 1 - i;          /* 1‑based index of the prime */
        seq_t p = (seq_t)get_prime(prime_index);/* t(n+1-i)   */
        sum += f_i * p;
    }

    return sum;
}
