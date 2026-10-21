/*
 * A204617  Multiplicative with a(p^e) = p^(e-1)*H(p). H(2) = 1, H(p) = p - 1
 *          if p == 1 (mod 4) and H(p) = p + 1 if p == 3 (mod 4).
 * https://oeis.org/A204617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} p^(e-1) * H(p) where H(2) = 1, H(p) = p-1 if p
 *            ≡ 1 (mod 4), H(p) = p+1 if p ≡ 3 (mod 4)
 * Range:     u8 n <= 188, u16 n <= 9695, u32 n <= 9695, u64 n <= 9695,
 *            u128 n <= 9695, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 1, a(0) just returns 1 (offset 1) */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;      /* local copy for factorisation */
    seq_t  result = 1;

    for (uint64_t p = 2; p <= m / p; ++p)   /* trial division up to sqrt(m) */
    {
        if (m % p != 0)
            continue;

        uint64_t e = 0;
        while (m % p == 0) {        /* count exponent */
            m /= p;
            ++e;
        }

        /* compute p^(e-1) */
        seq_t powterm = 1;
        for (uint64_t i = 1; i < e; ++i)
            powterm *= (seq_t)p;

        /* compute H(p) */
        seq_t h = (seq_t)((p == 2) ? 1 : ((p % 4 == 1) ? p - 1 : p + 1));

        result *= powterm;
        result *= h;
    }

    /* if a prime factor > sqrt(original n) remains */
    if (m > 1) {
        uint64_t p = m;
        seq_t h = (seq_t)((p == 2) ? 1 : ((p % 4 == 1) ? p - 1 : p + 1));
        result *= h;             /* e = 1 so p^(e-1) = 1 */
    }

    return result;
}
