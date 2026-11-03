/*
 * A160217  Minimal increasing sequence with a(1)=3 and the property that a(n)
 *          and n are both in or both not in A003159.
 * https://oeis.org/A160217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 1 if n is NOT in A003159 * a(n) = 2*n + 2 if n IS
 *            in A003159 * * A003159 consists of the positive integers n with
 *            an odd 2‑adic valuation, * i.e. n is even and v₂(n) is odd.
 *            Equivalently, * n = 2^(2k+1) * m with m odd (k ≥ 0). This simple
 *            parity test allows the * sequence to be compu...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 66 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute whether n belongs to A003159 */
static bool in_A003159(idx_t n)
{
    /* Odd numbers are never in A003159 */
    if (n & 1ULL) return false;

    /* Count trailing zeros (the 2‑adic valuation) */
    unsigned v = 0;
    idx_t t = n;
    while ((t & 1ULL) == 0) {
        t >>= 1;
        ++v;
    }
    /* In A003159 iff the valuation is odd */
    return (v & 1U) != 0;
}

seq_t a(idx_t n)
{
    /* n is assumed ≥ 1 */
    seq_t val = (seq_t)n << 1;           /* 2*n in seq_t arithmetic */
    if (in_A003159(n))
        val += (seq_t)2;                 /* 2*n + 2 */
    else
        val += (seq_t)1;                 /* 2*n + 1 */
    return val;
}
