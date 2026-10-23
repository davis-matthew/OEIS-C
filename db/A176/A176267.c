/*
 * A176267  a(n) = binomial(prime(n),s)/prime(n) where s is the sum of the
 *          decimal digits of prime(n).
 * https://oeis.org/A176267
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = binomial(prime(n),s) / prime(n) where s is the sum of
 *            decimal digits of prime(n)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 14, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 5: a(5) is the first term.
       For n < 5, behavior is unspecified; return 0. */
    if (n < 5) {
        return 0;
    }

    /* Retrieve the nth prime.  For n up to 10 000 this fits in uint32_t. */
    uint32_t p = get_prime(n);

    /* Compute the sum of the decimal digits of p. */
    unsigned s = 0;
    uint32_t tmp = p;
    while (tmp) {
        s += tmp % 10;
        tmp /= 10;
    }

    /* Compute binomial(p, s) using the supplied big‑integer routine. */
    seq_t bin = binomial((idx_t)p, (idx_t)s);

    /* Divide by p (always exact for prime p and 0 < s < p). */
    seq_t res = bin / (seq_t)p;

    return res;
}
