/*
 * A204125  Symmetric matrix based on f(i,j)=(i if i=j and 1 otherwise), by
 *          antidiagonals.
 * https://oeis.org/A204125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is even or n ≠ 2k(k‑1)+1 a(n) = k if n = 2k(k‑1)+1
 *            (k ≥ 1)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for seq_t (unsigned).  Works for 8, 16, 32, 64 and 128‑bit
   integers, since mid*mid is always ≤ x. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0, high = x;
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;           /* ceil((low+high)/2) */
        /* Avoid overflow: mid <= x / mid  <=> mid*mid <= x */
        if (mid <= x / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Position 1,3,5,... are the only candidates for non‑unit values. */
    if ((n & 1ULL) == 0)            /* even index → 1 */
        return 1;

    /* t = d(d‑1)  where n = 2t + 1  */
    seq_t tn   = (seq_t)n;
    seq_t t    = (tn - 1) / 2;
    seq_t disc = 1 + (t << 2);      /* 1 + 4*t = (2d‑1)² if d exists */

    seq_t s = isqrt_seq(disc);
    if (s * s != disc)              /* disc not a perfect square → 1 */
        return 1;

    /* s = 2d‑1   (always odd) → d = (1+s)/2 */
    seq_t d = (1 + s) >> 1;
    return d;
}
