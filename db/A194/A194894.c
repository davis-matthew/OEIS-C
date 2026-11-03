/*
 * A194894  The number of the ordered triples (A,B,C) satisfying the system of
 *          the modular relations {A*B - B*A = C, B*C - C*B = A, C*A - A*C =
 *          B}, where A,B,C are distinct 2 X 2 matrices over Z(n).
 * https://oeis.org/A194894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if n is a power of 2; otherwise let n = 2^k * ∏
 *            p_i^{e_i} (p_i odd primes), then a(n) = ∏ ( (p_i^2-1) *
 *            p_i^{3e_i-2} + 1 ) - 1. Uses multiplicativity:
 *            a(mn)=a(m)+a(n)+a(m)a(n), which is equivalent to
 *            (a(m)+1)(a(n)+1) = a(mn)+1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   ipow_i32, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"
#include "../utils/ipow_idx.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;                   /* a(1)=0 */
    /* strip powers of 2 */
    idx_t m = n;
    while ((m & 1) == 0) m >>= 1;
    if (m == 1) return (seq_t)0;                   /* n is a power of 2 */

    seq_t g = (seq_t)1;                            /* g = Π (a_p(e)+1) */
    /* factor the odd part m */
    idx_t d = 3;
    while (d * d <= m) {
        if (m % d == 0) {
            int e = 0;
            while (m % d == 0) { m /= d; ++e; }
            seq_t p = (seq_t)d;
            seq_t p_sq = p * p;                    /* p^2 */
            seq_t factor = p_sq - (seq_t)1;        /* p^2-1 */
            /* compute p^(3e-2) */
            seq_t p_pow = ipow_idx(p, 3 * (idx_t)e - 2);
            seq_t a_pe = factor * p_pow;           /* a(p^e) */
            g *= (a_pe + (seq_t)1);
        }
        d += 2;
    }
    if (m > 1) {                                   /* remaining prime */
        seq_t p = (seq_t)m;
        int e = 1;
        seq_t p_sq = p * p;                        /* p^2 */
        seq_t factor = p_sq - (seq_t)1;            /* p^2-1 */
        seq_t p_pow = ipow_idx(p, 3 * (idx_t)e - 2);
        seq_t a_pe = factor * p_pow;
        g *= (a_pe + (seq_t)1);
    }
    /* a(n) = g - 1 */
    return g - (seq_t)1;
}
