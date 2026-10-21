/*
 * A098591  a(k) contains primality information for the numbers in the
 *          interval (k*30,...,(k+1)*30) packed into one byte using the fact
 *          that only numbers == 1, 7, 11, 13, 17, 19, 23, 29 mod 30 can be
 *          prime.
 * https://oeis.org/A098591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..7} (2^k) * isprime(30*n + offset(k)), where
 *            offset = {1,7,11,13,17,19,23,29}. The term is the packed
 *            representation of the primality of the 8 admissible numbers in
 *            the interval [30*n, 30*n+30)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                /* for bool */
#include "../utils/is_prime.h"      /* bool is_prime(idx_t n) */

/* Compute a(n) for all n ≥ 1.  The function is stateless and works
   for any seq_t size (8, 16, 32, 64 or larger). */
seq_t a(idx_t n)
{
    /* Offsets of admissible residues modulo 30. */
    static const unsigned offsets[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    /* Return 0 for out‐of‐range input; behaviour for n<1 is
       unspecified, but returning 0 is harmless. */
    if (n < 1)
        return 0;

    /* Base of the current 30‑block: 30*n. */
    idx_t base = n * (idx_t)30;

    seq_t value = 0;

    for (unsigned k = 0; k < 8; ++k)
    {
        idx_t cand = base + (idx_t)offsets[k];
        if (is_prime(cand))
            value |= ((seq_t)1 << k);
    }

    return value;
}
