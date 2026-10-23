/*
 * A190723  Numbers m for which A055778(m) > A055778(m-1).
 * https://oeis.org/A190723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n-1)-th element of the union of B1(k) = ⌊k·φ⌋ + 2k
 *            (k ≥ 1) B2(k) = 4·⌊k·φ⌋ + 3k + 1 (k ≥ 0) where φ = (1+√5)/2.
 *            a(1) = 1. The union is taken with duplicates removed
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned integer (return floor(sqrt(x))). */
static inline unsigned __int128
isqrt128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = 1;
    while (hi * hi <= x) {
        hi <<= 1;
    }
    while (lo < hi) {
        unsigned __int128 mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

/* Floor{k·φ} using only integer arithmetic. */
static inline seq_t
phi_floor(idx_t k)
{
    if (k == 0) return 0;
    unsigned __int128 kk = k;
    unsigned __int128 prod = 5u * kk * kk;          /* 5·k² */
    unsigned __int128 sq  = isqrt128(prod);         /* ⌊k·√5⌋  */
    unsigned __int128 res = (kk + sq) >> 1;         /* ⌊k·φ⌋   */
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 */

    /* We need the (n‑1)-th element (1‑based) of the merged sequence. */
    idx_t target = n - 1;
    idx_t i = 1;   /* index for B1 (starts at 1) */
    idx_t j = 0;   /* index for B2 (starts at 0) */
    seq_t val = 0;
    idx_t cnt = 0; /* number of union elements generated so far */

    while (1) {
        seq_t b1 = phi_floor(i);
        b1 += (seq_t)(2 * i);                 /* B1(i) */

        seq_t b2 = phi_floor(j);
        b2 = 4 * b2 + (seq_t)(3 * j) + 1;     /* B2(j) */

        if (b1 < b2) {
            val = b1;
            ++i;
        } else if (b2 < b1) {
            val = b2;
            ++j;
        } else /* equal – skip duplicate */ {
            val = b1;
            ++i;
            ++j;
        }

        ++cnt;
        if (cnt == target) {
            return 1 + val;
        }
    }
}
