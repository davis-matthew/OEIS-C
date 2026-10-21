/*
 * A063041  Image of n under Collatz-2 map, a generalization of the classical
 *          '3x+1' - function: instead of dividing an even number by 2 a
 *          nonprime will be divided by its smallest prime factor and a prime
 *          will be multiplied not by 3 but by its prime-predecessor, before
 *          one is added.
 * https://oeis.org/A063041
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is prime then n * pp(n) + 1 else n / lpf(n) where
 *            pp(n) = largest prime < n (pp(2) = 1) lpf(n) = smallest prime
 *            factor of n
 * Range:     u8 n <= 18, u16 n <= 262, u32 n <= 9213, u64 n <= 9213,
 *            u128 n <= 9213, big any n
 * Helpers:   is_prime, lpf, prev_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/lpf.h"
#include "../utils/prev_prime.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 2) return (seq_t)3;          /* pp(2) = 1, so 2*1+1 = 3 */

    if (is_prime(n))
    {
        idx_t pp = prev_prime(n);            /* n > 2 guarantees at least prime 2 below */
        return (seq_t)n * (seq_t)pp + (seq_t)1;
    }
    else
    {
        idx_t smallest = lpf(n);
        return (seq_t)n / (seq_t)smallest;
    }
}
