/*
 * A011776  a(1) = 1; for n > 1, a(n) is defined by the property that n^a(n)
 *          divides n! but n^(a(n)+1) does not.
 * https://oeis.org/A011776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, nice
 * Formula:   a(n) = the largest k such that n^k divides n!. Computed as a(n)
 *            = min_{p^e‖n} floor(v_p(n!)/e), where v_p(n!) = Σ_{j≥1} ⌊n/p^j⌋
 *            is the exponent of prime p in n!
 * Range:     u8 n <= 1439, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1, and n=0 is not used by the OEIS offset */
        return (seq_t)1;

    idx_t min_k = (idx_t)-1;   /* start with maximum possible idx_t value */
    idx_t m = n;               /* remaining part to factor */

    /* trial division to factor n */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            /* count exponent e of prime p in n */
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute v_p(n!) */
            idx_t vp = 0;
            idx_t t = n;
            while (t) {
                t /= p;
                vp += t;
            }
            idx_t k = vp / e;
            if (k < min_k)
                min_k = k;
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (m > 1) {
        idx_t p = m;     /* remaining prime */
        idx_t e = 1;     /* exponent is 1 */
        idx_t vp = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            vp += t;
        }
        idx_t k = vp / e;          /* just vp */
        if (k < min_k)
            min_k = k;
    }

    return (seq_t)min_k;
}
