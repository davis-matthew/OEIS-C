/*
 * A072994  Number of solutions to x^n==1 (mod n), 1<=x<=n.
 * https://oeis.org/A072994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{p^e||n} gcd(n, φ(p^e)) with φ(p^e)=p^{e-1}(p−1)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of solutions to x^n ≡ 1 (mod n), for 1 ≤ x ≤ n. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n = 0, but safe */

    seq_t result = 1;
    idx_t m = n;

    for (idx_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            idx_t e = 0;
            while (m % d == 0) {
                m /= d;
                ++e;
            }

            /* φ(p^e) = p^{e-1} * (p−1) */
            seq_t pow = 1;
            for (idx_t i = 1; i < e; ++i)
                pow *= (seq_t)d;
            seq_t phi = pow * (seq_t)(d - 1);

            seq_t g = gcd((seq_t)n, phi);
            result *= g;
        }
    }

    /* If a prime factor larger than sqrt(n) remains. */
    if (m > 1) {
        seq_t phi = (seq_t)(m - 1);      /* φ(m) = m−1 for prime m */
        seq_t g   = gcd((seq_t)n, phi);
        result *= g;
    }

    return result;
}
