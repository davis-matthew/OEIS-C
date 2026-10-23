/*
 * A111419  a(n) is the smallest positive integer for which Fibonacci(n +
 *          a(n)) == Fibonacci(n) (mod n).
 * https://oeis.org/A111419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A111419:
   a(n) is the smallest positive integer such that
   Fibonacci(n + a(n)) ≡ Fibonacci(n)  (mod n)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/fib_mod.h"

seq_t a(idx_t n)
{
    /* Handle trivial case n == 1 */
    if (n == 1U) return (seq_t)1;

    seq_t mod = (seq_t)n;
    seq_t init_f = fib_mod(n, n);
    seq_t next_f = fib_mod(n + 1, n);

    seq_t cur_f = init_f;
    seq_t cur_next_f = next_f;

    seq_t a_val = 1;
    while (true)
    {
        /* Advance one Fibonacci step: F(k+2) = F(k+1) + F(k) mod n */
        seq_t tmp = cur_f + cur_next_f;
        if (tmp >= mod) tmp -= mod;   /* fast mod for small n */
        cur_f = cur_next_f;
        cur_next_f = tmp;

        if (cur_f == init_f) return a_val;
        ++a_val;
    }
}
