/*
 * A214056  Least m>0 such that 2^n+m and n-m have a common divisor > 1.
 * https://oeis.org/A214056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 282, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A214056: a(n) = least m>0 such that gcd(2^n + m, n - m) > 1
   The code uses a simple search over m = 1..n and computes gcd with
   modular exponentiation to avoid handling large powers of 2. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute 2^exp mod mod (mod > 0).  exp is small (<= 10000). */
static seq_t pow2_mod(idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t base = 2 % mod;      /* base = 2, mod > 1 in callers */
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t d = n - m;

        if (d == 0) {              /* n-m == 0 → gcd(2^n + m, 0) = 2^n + m > 1 */
            return (seq_t)m;
        }
        if (d == 1) {              /* gcd with 1 can never be > 1 */
            continue;
        }

        seq_t mod = (seq_t)d;
        seq_t pow_mod = pow2_mod(n, mod);                /* 2^n mod d */
        seq_t c = (pow_mod + (seq_t)m) % mod;            /* (2^n + m) mod d */
        if (c == 0) {                                   /* d | (2^n + m) */
            return (seq_t)m;
        }

        seq_t g = gcd(c, mod);
        if (g > (seq_t)1) {
            return (seq_t)m;
        }
    }
    /* This point is never reached because m = n always works */
    return (seq_t)0;
}
