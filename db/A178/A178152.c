/*
 * A178152  The number of quadratic residues (mod p) greater than p/2, where
 *          p=prime(n).
 * https://oeis.org/A178152
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of non‑zero quadratic residues modulo the nth
 *            prime that are > p/2
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(2) = 0, a(3) = 1, ... as defined by OEIS A178152. */

#include "../utils/get_prime.h"   /* Provides: uint32_t get_prime(idx_t k); */

seq_t a(idx_t n)
{
    /* Offsets: a(2) is defined; values for n < 2 are irrelevant. */
    if (n < 2)
        return 0;

    /* Fetch the nth prime. get_prime returns uint32_t, safe to cast to idx_t. */
    idx_t p = get_prime(n);                 /* p = prime(n)   */
    idx_t half = p / 2;                     /* floor(p/2)     */
    idx_t upper = (p - 1) / 2;              /* count distinct residues */

    idx_t count = 0;                        /* count of residues > half */
    for (idx_t k = 1; k <= upper; ++k)
    {
        idx_t r = (k * k) % p;              /* k^2 mod p  (distinct residues for k <= upper) */
        if (r > half)
            ++count;
    }
    return (seq_t)count;
}
