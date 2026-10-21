/*
 * A060634  Union of Fibonacci numbers and prime numbers.
 * https://oeis.org/A060634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th integer that is either a prime or a Fibonacci
 *            number
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Simple primality test for small numbers */
static bool is_prime(idx_t x)
{
    if (x <= 1) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;
    for (idx_t d = 3; (unsigned __int128)d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* The sequence A060634 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* a(1) = 1 */

    idx_t count = 1;                /* value 1 already counted */

    /* Fibonacci state (unique Fibonacci numbers) */
    seq_t fib_prev = 1;  /* previous distinct Fibonacci */
    seq_t fib_curr = 2;  /* next distinct Fibonacci (fib_prev + fib_curr) */
    seq_t next_fib = fib_prev;            /* next Fibonacci number to check */

    for (idx_t cand = 2;; ++cand) {
        /* advance Fibonacci to the first >= candidate */
        while (next_fib < cand) {
            seq_t temp = fib_curr;
            fib_curr = fib_prev + fib_curr;
            fib_prev = temp;
            next_fib = fib_prev;
        }

        bool is_fib = (next_fib == cand);
        if (is_fib) {
            /* advance Fibonacci after consuming this number */
            seq_t temp = fib_curr;
            fib_curr = fib_prev + fib_curr;
            fib_prev = temp;
            next_fib = fib_prev;
        }

        bool prime = is_prime(cand);

        if (prime || is_fib) {
            ++count;
            if (count == n) {
                return (seq_t)cand;
            }
        }
    }
}
