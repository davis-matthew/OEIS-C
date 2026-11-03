/*
 * A105955  a(n) = Fibonacci(n) mod 11.
 * https://oeis.org/A105955
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 117 terms through n = 911 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    return fib_mod(n, 11);
}
