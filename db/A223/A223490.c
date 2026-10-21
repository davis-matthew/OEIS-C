/*
 * A223490  Smallest Fermi-Dirac factor of n.
 * https://oeis.org/A223490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n / A302792(n) (Smallest Fermi‑Dirac factor of n)
 *            Computation: 1. Let n = 2^k · m with m odd. 2. If m = 1 (pure
 *            power of two): – if k ≡ 0 (mod 4) → a = n – if k ≡ 2 (mod 4) → a
 *            = 4 – otherwise (k odd) → a = 2 3. If k is odd → a = 2. 4. If k
 *            is even and k > 0: • Find the smallest odd prime...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)n;
    }

    /* Separate n into 2^k * m, with m odd */
    idx_t tmp = n;
    idx_t k = 0;                /* exponent of 2 */
    while ((tmp & 1ULL) == 0) {
        ++k;
        tmp >>= 1;
    }
    seq_t odd = (seq_t)tmp;     /* odd part (>= 1) */

    /* Pure powers of two */
    if (odd == 1) {
        idx_t mod4 = k & 3;
        if (mod4 == 0) return (seq_t)n;    /* n itself */
        if (mod4 == 2) return (seq_t)4;
        return (seq_t)2;
    }

    /* When k is odd, the smallest Fermi‑Dirac factor is 2 */
    if (k & 1) {
        return (seq_t)2;
    }

    /* k is even (possibly zero) and odd part > 1 */
    /* Find the smallest odd prime factor of odd whose exponent in odd is odd */
    seq_t p = 0;                 /* candidate prime factor */
    idx_t temp = (idx_t)odd;
    for (idx_t d = 3; d * d <= temp; d += 2) {
        if (temp % d == 0) {
            idx_t cnt = 0;
            while (temp % d == 0) {
                temp /= d;
                ++cnt;
            }
            if ((cnt & 1) && (p == 0 || d < p)) {
                p = d;
            }
        }
    }
    if (temp > 1) {              /* remaining prime factor (exponent 1, odd) */
        if (p == 0 || temp < p) {
            p = temp;
        }
    }
    if (p == 0) {
        p = odd;                 /* all exponents even */
    }

    /* If k == 0 (odd n) the answer is just p */
    if (k == 0) {
        return p;
    }

    /* k > 0 and even: compare with 2^k */
    seq_t q = ((seq_t)1) << k;
    return (p < q) ? p : q;
}
