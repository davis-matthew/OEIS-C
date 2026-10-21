/*
 * A082064  Greatest common prime-divisor of phi(n) and sigma(n) = A000203(n);
 *          a(n)=1 if no common prime-divisor exists.
 * https://oeis.org/A082064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = greatest common prime divisor of φ(n) and σ(n), with
 *            a(n)=1 when φ(n) and σ(n) share no common prime divisor
 * Range:     u8 n <= 793, u16 n <= 9635, u32 n <= 9635, u64 n <= 9635,
 *            u128 n <= 9635, big any n
 * Helpers:   gcd, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"        /* seq_t phi(idx_t m); */
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/gcd.h"        /* seq_t gcd(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* Compute φ(n) and σ(n) */
    seq_t phi_val   = phi(n);
    seq_t sigma_val = sigma_of(n);

    /* Common divisor of φ(n) and σ(n) */
    seq_t g = gcd(phi_val, sigma_val);

    /* If there is no common prime divisor, return 1 */
    if (g <= 1) return (seq_t)1;

    /* Find the greatest prime factor of g */
    uint64_t g64   = (uint64_t)g;
    uint64_t temp  = g64;
    uint64_t largest = 1;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            largest = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) largest = temp;  /* the remaining prime factor */

    return (seq_t)largest;
}
