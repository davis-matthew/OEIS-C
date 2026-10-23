/*
 * A177454  a(n) = (binomial(2*p,p) - 2)/p, where p = prime(n).
 * https://oeis.org/A177454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(2*p, p) - 2) / p, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   binomial, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n < 1, return 0 */

    /* Get the n-th prime */
    uint32_t p_uint = get_prime(n);      /* p fits in 32 bits for n <= ~10^6 */
    idx_t p = (idx_t)p_uint;             /* promote to idx_t for binomial */

    /* Compute binomial(2*p, p) */
    idx_t two_p = p << 1;                /* 2*p, safe within idx_t */
    seq_t bin = binomial(two_p, p);

    /* Compute (bin - 2) / p */
    bin -= (seq_t)2;
    bin /= p;

    return bin;
}
