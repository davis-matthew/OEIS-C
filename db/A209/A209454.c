/*
 * A209454  a(n) = Pell(n)*A033719(n) for n>=1, with a(0)=1, where A033719
 *          lists the coefficients in theta_3(q)*theta_3(q^7).
 * https://oeis.org/A209454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Pell(n) * A033719(n) (a(0)=1)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 27, u64 n <= 52, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for a 64‑bit unsigned number (floor). */
static uint64_t int_sqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v, ans = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Pell number P(n): P0=0, P1=1, Pn=2*P(n‑1)+P(n‑2). */
static seq_t pell(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t p0 = (seq_t)0;
    seq_t p1 = (seq_t)1;
    seq_t p2 = 0;
    for (idx_t i = 2; i <= n; ++i) {
        p2 = (seq_t)2 * p1 + p0;  /* compute 2*P(i‑1)+P(i‑2) */
        p0 = p1;
        p1 = p2;
    }
    return p1;
}

/* Count integer pairs (k,l) with k² + 7·l² = n.  All signs are counted. */
static seq_t a033719(idx_t n)
{
    uint64_t lmax = int_sqrt_u64((uint64_t)n / 7ULL);
    seq_t total = 0;
    for (uint64_t l = 0; l <= lmax; ++l) {
        uint64_t m = (uint64_t)n - 7ULL * l * l;
        uint64_t s = int_sqrt_u64(m);
        if (s * s == m) {
            seq_t cnt_k = (s == 0) ? 1 : 2;  /* include ±k */
            seq_t cnt_l = (l == 0) ? 1 : 2;  /* include ±l */
            total += cnt_k * cnt_l;
        }
    }
    return total;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t p = pell(n);
    seq_t c = a033719(n);
    return p * c;
}
