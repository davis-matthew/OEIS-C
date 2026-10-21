/*
 * A082071  Smallest common prime-divisor of phi(n) = A000010(n) and
 *          sigma_2(n) = A001157(n); a(n)=1 if no common prime-divisor exists.
 * https://oeis.org/A082071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of gcd(phi(n), sigma_2(n)), or 1
 *            if gcd = 1
 * Range:     u8 n <= 35, u16 n <= 5407, u32 n <= 9436, u64 n <= 9436,
 *            u128 n <= 9436, big any n
 * Helpers:   gcd, phi, sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/phi.h"
#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    seq_t phi_val   = phi(n);          /* φ(n) */
    seq_t sigma2_val= sigma_k(n, 2);   /* σ₂(n) */
    seq_t g = gcd(phi_val, sigma2_val);

    if (g == 1) return 1;              /* no common prime divisor */

    /* g fits into uint64_t for the typical n-range; cast safely */
    uint64_t gv = (uint64_t)g;

    /* find the smallest prime factor by trial division */
    if (gv % 2 == 0) return 2;
    for (uint64_t d = 3; d * d <= gv; d += 2)
        if (gv % d == 0)
            return d;
    return (seq_t)gv;                  /* gv itself is prime */
}
