/*
 * A122410  a(n) is the sum of j's for those k's, 1 <= k <= n, where gcd(k,n)
 *          = p^j, p = prime.
 * https://oeis.org/A122410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n} ((p^v - 1)/(p - 1)) * φ(n / p^v) where v =
 *            v_p(n) is the exponent of prime p in n. This follows from the
 *            equivalent expression a(n) = Sum_{d|n, d prime power} v_d *
 *            φ(n/d), with v_d = exponent of the prime contained in d
 * Range:     u8 n <= 295, u16 n <= 7980, u32 n <= 7980, u64 n <= 7980,
 *            u128 n <= 7980, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/phi.h"   /* defines: static seq_t phi(idx_t m) */

seq_t a(idx_t n)
{
    if (n == 0)          /* nothing is defined below the offset; return 0 */
        return 0;

    seq_t result = 0;
    uint64_t nn   = (uint64_t)n;   /* keep original value for later division */
    uint64_t temp = nn;            /* temporary copy for factorisation */

    /* Trial division up to sqrt(temp).  For each prime factor p of nn
       compute its exponent v and the contribution to the sum. */
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            uint64_t v   = 0;
            uint64_t pv  = 1;          /* pv = p^v */
            while (temp % p == 0) {
                temp /= p;
                ++v;
                pv *= p;
            }
            /* ((p^v - 1)/(p - 1)) * φ(nn / p^v) */
            seq_t term1  = ((seq_t)pv - 1) / ((seq_t)p - 1);
            seq_t phi_v  = phi((idx_t)(nn / pv));
            result += term1 * phi_v;
        }
    }

    /* If a prime factor greater than sqrt(nn) remains, it occurs with
       exponent 1 (otherwise we would have found it earlier). */
    if (temp > 1) {
        uint64_t p  = temp;            /* remaining prime */
        uint64_t pv = p;               /* p^1 */
        seq_t term1 = ((seq_t)pv - 1) / ((seq_t)p - 1);   /* equals 1 */
        seq_t phi_v = phi((idx_t)(nn / pv));
        result += term1 * phi_v;
    }

    return result;
}
