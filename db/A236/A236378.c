/*
 * A236378  Difference between n and the result obtained when n is
 *          remultiplied from N to GF(2)[X]: a(n) = n - A234741(n).
 * https://oeis.org/A236378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - carryless_product_of_prime_factors(n)
 * Range:     u8 n <= 323, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   mul_no_carry
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The carryless product is computed by multiplying the prime factors
   of n one by one using carry‑less (GF(2)[X]) multiplication. */

#include <stdint.h>
#include "../utils/mul_no_carry.h"

/*  a(n) : difference between n and the carry‑less product of its prime factors. */
seq_t a(idx_t n)
{
    /* Definition: a(0) = 0. */
    if (n == 0)
        return 0;

    seq_t product = 1;          /* carry‑less product accumulator */

    /* Factor n using trial division.  For large n this is still
       fast enough for the intended range (~10 000).              */
    uint64_t m = (uint64_t)n;

    /* Handle factor 2 separately. */
    while ((m & 1ULL) == 0) {
        product = mul_no_carry(product, (seq_t)2);
        m >>= 1;
    }

    /* Odd factors. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            while (m % p == 0) {
                product = mul_no_carry(product, (seq_t)p);
                m /= p;
            }
        }
    }

    /* If remaining m > 1, it is a prime factor. */
    if (m > 1)
        product = mul_no_carry(product, (seq_t)m);

    /* Return the difference n - product. */
    return ((seq_t)n) - product;
}
