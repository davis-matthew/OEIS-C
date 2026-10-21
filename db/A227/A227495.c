/*
 * A227495  The hyper-Wiener index of the nanostar dendrimer D_n, defined
 *          pictorially as NS[n] in the Yang et al. reference.
 * https://oeis.org/A227495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -140340 + 2^n*(82764*n^2 - 268092*n - 1159017) +
 *            4^n*(156816*n^2 - 719928*n + 1304154) Sequence offset 1: a(1)
 *            corresponds to n = 0 in the formula
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* offset 1: use m = n-1  (m = 0 for n = 1) */
    idx_t m = n - 1;

    /* For the first few values the 2^n coefficient is negative;
       compute them using ordinary signed 64‑bit arithmetic. */
    if (n <= 5) {
        int64_t mm   = (int64_t)m;          /* m = 0..4 */
        int64_t term2 = 82764LL * mm * mm
                        - 268092LL * mm
                        - 1159017LL;
        int64_t term4 = 156816LL * mm * mm
                        - 719928LL * mm
                        + 1304154LL;
        int64_t pow2m = 1LL << mm;          /* 2^m, m <= 4 */
        int64_t pow4m = 1LL << (2 * mm);    /* 4^m */
        int64_t res   = -140340LL
                         + pow2m * term2
                         + pow4m * term4;
        return (seq_t)res;
    }

    /* For m >= 6 the polynomial multiplying 2^m is positive. */
    seq_t pow2m = pow2(m);      /* 2^m   */
    seq_t pow4m = pow4(m);      /* 4^m   */

    seq_t m_sq  = (seq_t)m * (seq_t)m;

    /* 82764*m^2 - 268092*m - 1159017    (positive for m >= 6) */
    seq_t term2 = (seq_t)82764 * m_sq;
    term2 -= (seq_t)268092 * (seq_t)m;
    term2 -= (seq_t)1159017;

    /* 156816*m^2 - 719928*m + 1304154   (positive for all m)   */
    seq_t term4 = (seq_t)156816 * m_sq;
    term4 -= (seq_t)719928 * (seq_t)m;
    term4 += (seq_t)1304154;

    seq_t result = pow2m * term2 + pow4m * term4;
    result -= (seq_t)140340;
    return result;
}
