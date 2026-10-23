/*
 * A111029  Magic products of 3 X 3 multiplicative magic squares.
 * https://oeis.org/A111029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 21, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The magic product a(n) is the n-th cube of an integer m such that
   m has at least two distinct prime factors, or m = p^k with k ≥ 4
   (i.e. m is not a prime nor a prime power of exponent 1, 2, or 3). */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* bool is_prime(uint64_t n); */

static bool is_valid_base(idx_t m)
{
    /* Reject primes */
    if (is_prime((uint64_t)m))
        return false;

    /* Find first prime divisor */
    idx_t temp = m;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            /* If m is a prime power with exponent ≤ 3, reject */
            if (temp == 1 && exp <= 3)
                return false;
            /* Otherwise m is valid (either composite with ≥2 primes
               or prime power with exponent ≥4) */
            return true;
        }
    }
    /* temp has no divisor <= sqrt(m).  Since m is not prime, it
       must be of the form p^k with k ≥ 4, thus valid. */
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 2; ; ++m) {
        if (is_valid_base(m)) {
            if (count == n) {
                seq_t val = (seq_t)m;
                return val * val * val;   /* m^3 */
            }
            ++count;
        }
    }
}
