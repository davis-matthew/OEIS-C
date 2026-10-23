/*
 * A017696  Denominator of sum of -16th powers of divisors of n.
 * https://oeis.org/A017696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n^16 / gcd(n^16, sigma_16(n)) where sigma_16(n) =
 *            sum_{d|n} d^16
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Helpers:   gcd, pow_uint, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Use the provided helpers: sigma_k, pow_uint, and gcd. */
#include "../utils/sigma_k.h"   /* seq_t sigma_k(idx_t m, uint8_t k);          */
#include "../utils/pow_uint.h"  /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/gcd.h"       /* seq_t gcd(seq_t a, seq_t b);               */

seq_t a(idx_t n)
{
    /* Compute n^16.  pow_uint expects a 64‑bit base, our n fits. */
    seq_t n_pow = pow_uint(n, 16);

    /* Compute sigma_16(n). */
    seq_t sigma = sigma_k(n, 16);

    /* Reduce the fraction sigma / n^16.
       The denominator after reduction is n^16 / gcd(n^16, sigma). */
    seq_t g = gcd(n_pow, sigma);
    return n_pow / g;
}
