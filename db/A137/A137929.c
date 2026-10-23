/*
 * A137929  If n = the k-th prime power (including 1), then a(n) = the (k+1)th
 *          prime power. If n = the k-th positive integer that is not a prime
 *          power, then a(n) = the (k+1)th positive integer that is not a
 *          prime power.
 * https://oeis.org/A137929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A137929: if n is a prime power (1 counted as one), a(n) is the next
   prime power; otherwise a(n) is the next integer that is not a prime
   power.  This implementation performs a linear search for the next
   integer of the same type as n.  The helper is_prime_power does a
   simple trial division; for n up to 10 000 this is easily fast enough. */

#include <stdbool.h>
#include <stdint.h>

/* Return true iff x is a prime power (1 counted as a prime power). */
static bool is_prime_power(idx_t x)
{
    if (x == 1) return true;           /* 1 is treated as a prime power */
    if (x < 2)  return false;          /* 0 is not a prime power */

    /* Find a prime factor of x. */
    idx_t p;
    for (p = 2; p <= x / p; ++p) {
        if (x % p == 0) break;
    }

    /* If no factor found, x is prime, hence a prime power. */
    if (p > x / p) return true;

    /* Remove all powers of p. */
    idx_t t = x;
    while (t % p == 0) t /= p;

    /* x is a prime power iff after division by its prime factor
       we are left with 1. */
    return t == 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    bool n_is_pp = is_prime_power(n);
    idx_t m = n + 1;
    for (;;) {
        if (is_prime_power(m) == n_is_pp) return (seq_t)m;
        ++m;
    }
}
