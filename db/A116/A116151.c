/*
 * A116151  a(n) = smallest positive integer x satisfying the system of
 *          congruences { x == 1 (mod 2), x == 2 (mod 3), x == 3 (mod 5), x ==
 *          5 (mod 7), ..., x == A008578(n) (mod A008578(n+1)) }.
 * https://oeis.org/A116151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least positive integer satisfying x ≡ 1 (mod 2), x ≡
 *            2 (mod 3), x ≡ 3 (mod 5), x ≡ 5 (mod 7), …, x ≡ A008578(n) (mod
 *            A008578(n+1)). The implementation applies the Chinese remainder
 *            theorem iteratively over the first n primes
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Undefined for n == 0; return 0 as a placeholder. */
    if (n == 0)
        return (seq_t)0;

    seq_t x = (seq_t)1;     /* current solution modulo m   */
    seq_t m = (seq_t)1;     /* current modulus (product so far) */

    for (idx_t i = 0; i < n; ++i) {
        /* p = the (i+1)-th prime; residue = previous prime
         *   (or 1 for the first congruence). */
        uint32_t p = get_prime(i + 1);
        seq_t mod     = (seq_t)p;
        seq_t residue = (i == 0) ? (seq_t)1 : (seq_t)get_prime(i);

        /* x ≡ r (mod p)  ⇢  (x + k·m) ≡ r (mod p)  ⇢
         *       k ≡ (r−x)·m⁻¹ (mod p)   (m⁻¹ ≡ m⁻¹ mod p)   */
        seq_t x_mod_p = x % mod;
        seq_t diff;
        if (residue >= x_mod_p) {
            diff = residue - x_mod_p;
        } else {
            /* wrap around: r + mod − x_mod_p */
            diff = residue + mod - x_mod_p;
        }

        seq_t m_mod_p = m % mod;
        seq_t inv = mod_pow(m_mod_p, (uint64_t)p - 2, mod);  /* inverse of m mod p */

        seq_t delta = (diff * inv) % mod;

        x = x + delta * m;
        m = m * mod;
    }

    return x;
}
