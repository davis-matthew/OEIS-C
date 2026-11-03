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
 * Validated: standard, pass, 96 terms through n = 9999 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A111419:
   a(n) is the smallest positive integer such that
   Fibonacci(n + a(n)) ≡ Fibonacci(n)  (mod n)
*/

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Fast doubling Fibonacci modulo using unsigned long long for
   intermediate arithmetic.  Returns F_n mod mod. */
static seq_t fib_mod(idx_t n, idx_t mod)
{
    if (n == 0) return 0;
    unsigned long long f = 0, fnext = 1;
    /* Iterate over the bits of n from most significant to least. */
    int highest = 63 - __builtin_clzll(n);
    for (int i = highest; i >= 0; --i) {
        unsigned long long a = f;
        unsigned long long b = fnext;
        unsigned long long two_b_mod = (b << 1) % mod;
        unsigned long long c = (a * ((two_b_mod + mod - a) % mod)) % mod;
        unsigned long long d = (a * a % mod + b * b % mod) % mod;
        if ((n >> i) & 1) {          /* odd n = 2k+1 */
            f = d % mod;
            fnext = (c + d) % mod;
        } else {                     /* even n = 2k   */
            f = c % mod;
            fnext = d % mod;
        }
    }
    return (seq_t)f;
}

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
