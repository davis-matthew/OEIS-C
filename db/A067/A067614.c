/*
 * A067614  a(n) is the second partial quotient in the simple continued
 *          fraction for sqrt(prime(n)).
 * https://oeis.org/A067614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*floor(sqrt(p))/ (p - floor(sqrt(p))^2))
 * Range:     u8 n <= 18, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p = prime(n) */

#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑based). */
    uint32_t prime_val = get_prime(n);

    /* Compute s = floor(sqrt(p)). */
    seq_t s = floor_sqrt((seq_t)prime_val);

    /* r = p - s^2.  The subtraction is safe because s^2 < p. */
    seq_t r = (seq_t)prime_val - s * s;

    /* a(n) = floor(2 * s / r). */
    seq_t result = ((seq_t)2 * s) / r;

    return result;
}
