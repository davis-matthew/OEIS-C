/*
 * A069862  Smallest k such that n divides the concatenation of numbers from
 *          (n+1) to (n+k), where (n+1) is on the most significant side.
 * https://oeis.org/A069862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n divides the concatenation of
 *            the integers from n+1 to n+k (base 10)
 * Range:     u8 n <= 36, u16 n <= 450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits_of.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n) {
    /* Special case: a(1) = 1 by definition. */
    if (n == 1) return (seq_t)1;

    seq_t mod = (seq_t)n;          /* modulus */
    seq_t rem = 0;                 /* current remainder modulo n */

    for (idx_t k = 1; ; ++k) {
        idx_t x = n + k;           /* the next integer to append */
        int d = digits_of(x);     /* number of decimal digits in x */

        /* (10^d) mod n */
        seq_t mult = mod_pow((seq_t)10, (uint64_t)d, mod);

        /* Append the digits of x to the current number modulo n */
        rem = (rem * mult + (seq_t)x) % mod;

        if (rem == 0) return (seq_t)k;
    }
}
