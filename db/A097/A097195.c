/*
 * A097195  Expansion of s(12)^3*s(18)^2/(s(6)^2*s(36)), where s(k) = eta(q^k)
 *          and eta(q) is Dedekind's function, cf. A010815. Then replace q^6
 *          with q.
 * https://oeis.org/A097195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = b(6*n+1) where b() is multiplicative with b(p^e) = e+1 if
 *            p ≡ 1 (mod 6), b(p^e) = (1+(-1)^e)/2 if p ≡ 5 (mod 6), and
 *            b(p^e)=0 for p=2,3. (See OEIS A097195) The function computes
 *            this value for any n ≥ 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute m = 6*n + 1; we use 128‑bit to avoid overflow for
       reasonably large n.  If m exceeds UINT64_MAX we simply
       return 0 (the sequence is undefined beyond this range). */
    unsigned __int128 m128 = (unsigned __int128)6 * n + 1;
    if (m128 > UINT64_MAX) {
        return (seq_t)0;
    }
    uint64_t m = (uint64_t)m128;

    seq_t result = (seq_t)1;      /* multiplicative identity */

    /* Factorisation by simple trial division.  For the range of n
       considered (up to ~10 000) m is at most about 6 × 10⁴+1,
       so the loop is trivial.  Even for larger values this
       algorithm remains practical below 10 000 calls. */
    uint64_t temp = m;
    for (uint64_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            unsigned exp = 0;
            while (temp % d == 0) {
                temp /= d;
                ++exp;
            }

            /* Only p ≡ 1 or 5 (mod 6) can occur because m ≡ 1 (mod 6). */
            if (d % 6 == 1) {
                result *= (seq_t)(exp + 1);
            } else if (d % 6 == 5) {
                if (exp & 1) {          /* odd exponent → factor 0 */
                    return (seq_t)0;
                }
                /* even exponent → factor 1, do nothing */
            } else {
                /* d is 2 or 3 – impossible for m ≡ 1 (mod 6),
                   but just in case handle it gracefully. */
                return (seq_t)0;
            }
        }
    }

    /* If a prime factor remains > 1, it has exponent 1. */
    if (temp > 1) {
        uint64_t p = temp;
        if (p % 6 == 1) {
            result *= (seq_t)2;       /* exp = 1 → exp+1 = 2 */
        } else if (p % 6 == 5) {
            return (seq_t)0;          /* odd exponent → 0 */
        } else {
            /* Should not happen for m ≡ 1 (mod 6) */
            return (seq_t)0;
        }
    }

    return result;
}
