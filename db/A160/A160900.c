/*
 * A160900  a(n) = the smallest positive multiple of n that has exactly a
 *          prime number of (non-leading) 0's in its binary representation.
 * https://oeis.org/A160900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): smallest positive multiple of n with a prime number of
   non‑leading zeros in its binary representation.  The function
   iteratively tests multiples of n until the number of zeros is a prime. */

#include <stdbool.h>
#include <stddef.h>

/* Small primality test for values up to a few hundred. */
static bool is_prime_uint(unsigned x)
{
    if (x < 2) return false;
    for (unsigned i = 2; i * i <= x; ++i)
        if (x % i == 0) return false;
    return true;
}

/* Counts the zeros in the binary representation of x.
   Assumes x > 0. */
static unsigned zeros_count(seq_t x)
{
    unsigned z = 0;
    while (x > 0) {
        if ((x & 1) == 0) ++z;
        x >>= 1;
    }
    return z;
}

/* a(n) as defined in OEIS A160900. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1, but guard for zero index */
    seq_t val = (seq_t)n;           /* first multiple (k = 1) */
    for (;;) {
        unsigned z = zeros_count(val);
        if (is_prime_uint(z))
            return val;
        val += (seq_t)n;            /* next multiple */
    }
}
