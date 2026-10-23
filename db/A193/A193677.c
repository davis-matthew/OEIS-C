/*
 * A193677  Number of negative zeros of minimal polynomials of 2*cos(Pi/n),
 *          n>=1.
 * https://oeis.org/A193677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = delta(n) - A193676(n) where delta(n) = 1 if n == 1 φ(n)
 *            if n is even (n>=2) φ(n)/2 if n is odd (n>=3) and A193676(n) =
 *            #{ k : 1 ≤ k ≤ ⌊n/2⌋, k is odd, gcd(k,n)=1 } (n≥1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_uint64.h"

/* compute φ(n) for n ≥ 0 (works for n ≤ UINT64_MAX)
   trial division is sufficient for the intended range (n up to ~10⁴) */
static uint64_t phi_uint64(uint64_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t result = n;
    for (uint64_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            while (n % p == 0) n /= p;
            result -= result / p;
        }
    }
    if (n > 1) result -= result / n;
    return result;
}

/* Euclidean algorithm for gcd, works for 64‑bit arguments */

/* --- sequence A193677 ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* delta(1)=1, no non‑negative root */

    uint64_t n64 = (uint64_t)n;

    /* degree delta(n) */
    uint64_t phi_n = phi_uint64(n64);
    seq_t delta = (seq_t)((n64 & 1ULL)     /* n odd  */
                     ? phi_n / 2ULL       /*      φ(n)/2  */
                     : phi_n);           /* n even => φ(n) */

    /* count non‑negative zeros: odd k ≤ n/2 with gcd(k,n)=1 */
    uint64_t half = n64 >> 1;
    uint64_t cnt  = 0;
    for (uint64_t k = 1; k <= half; ++k) {
        if ((k & 1ULL) && (gcd_uint64(k, n64) == 1ULL))
            ++cnt;
    }

    return delta - (seq_t)cnt;   /* negative zeros = delta - non‑negative */
}
