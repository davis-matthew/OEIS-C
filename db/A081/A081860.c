/*
 * A081860  a(n) = Sum_{k=0..n-1} sigma(2k+1)*sigma_3(n-k).
 * https://oeis.org/A081860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} sigma(2k+1)*sigma_3(n-k) = (sigma_5(2n+1)
 *            - sigma(2n+1))/240 (sigma_m = sum of d^m over all positive
 *            divisors d of n)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 125, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9075 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sigma_1 and sigma_5 of n (n is odd, n <= 20001 for n <= 10000) */
static void
compute_sigma1_sigma5(idx_t n, __int128 *sig1, __int128 *sig5)
{
    __int128 s1 = 0, s5 = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;
            s1 += d;
            s5 += (__int128)d * d * d * d * d;
            if (q != d) {
                s1 += q;
                s5 += (__int128)q * q * q * q * q;
            }
        }
    }
    *sig1 = s1;
    *sig5 = s5;
}

/* External integer types defined by the build system */
seq_t a(idx_t n);

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* value not defined for n < 1, but safe */

    idx_t m = 2 * n + 1;           /* 2n+1 is odd */

    __int128 sigma1, sigma5;
    compute_sigma1_sigma5(m, &sigma1, &sigma5);

    __int128 diff = sigma5 - sigma1;
    __int128 result = diff / 240;  /* exact division, diff is multiple of 240 */

    return (seq_t)result;          /* cast after division to preserve value */
}
