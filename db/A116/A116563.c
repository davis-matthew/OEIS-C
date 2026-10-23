/*
 * A116563  a(n) is the genus of the modular curve X_0(p) for p = prime(n).
 * https://oeis.org/A116563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001617(prime(n)) = genus of the modular curve X0(p)
 *            where p = prime(n). Depending on p mod 12 the value is: - p ≡ 1
 *            (mod 12) : (p-13)/12 - p ≡ 5 (mod 12) : (p-5)/12 - p ≡ 7 (mod
 *            12) : (p-7)/12 - p ≡ 11 (mod 12): (p+1)/12 For p = 2,3 the
 *            result is 0
 * Range:     u8 n <= 438, u16 n <= 7058, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Retrieve the n‑th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute p mod 12. */
    uint32_t r = p % 12U;

    /* Apply the appropriate formula. */
    switch (r) {
        case 1:  return ((seq_t)p - 13) / 12;
        case 5:  return ((seq_t)p - 5)  / 12;
        case 7:  return ((seq_t)p - 7)  / 12;
        case 11: return ((seq_t)p + 1)  / 12;
        default: return 0;   /* primes 2,3,5,7,13,... handled above */
    }
}
