/*
 * A024568  a(n) = [ n/{n*r} ], where r = (1 + sqrt(5))/2 and {x} := x - [ x
 *          ].
 * https://oeis.org/A024568
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊ n / { n*r } ⌋ , r = (1+√5)/2
 * Range:     u8 n <= 12, u16 n <= 232, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   less_than_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic.  It
   1. computes s = ⌊ n*√5 ⌋ as ⌊√(5*n²)⌋ with an
      128‑bit integer square root routine;
   2. decides whether n+s is even or odd;
   3. evaluates a(n) as ⌊ (2n) / (n*√5 − s_use) ⌋,
      where  s_use = s         if n+s even
                     s – 1     if n+s odd;
   4. finds the floor by a binary search with a test
      that compares (α√5+β)/γ to an integer using only
      128‑bit integer operations. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/less_than_value.h"

/* fast integer square root for a 128‑bit unsigned integer  */
static unsigned __int128 isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 64;
    unsigned __int128 res = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= x) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of OEIS range */

    /* 1.  compute s = ⌊ n*√5 ⌋ as ⌊√(5*n²)⌋  */
    unsigned __int128 n128 = n;
    unsigned __int128 v    = n128 * n128 * (unsigned __int128)5;
    unsigned __int128 s    = isqrt_u128(v);

    /* 2.  parity of n+s */
    bool odd = ((n128 + s) & 1) != 0;
    unsigned __int128 s_use = odd ? (s - 1) : s;  /* use s or s-1 */

    /* 3.  binary search for floor value  */
    unsigned __int128 max_val = (unsigned __int128)5 * n128 * n128 + 1;  /* safe upper bound */
    unsigned __int128 lo = 0, hi = max_val;
    while (lo + 1 < hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        if (less_than_value(n, s_use, mid))
            hi = mid;
        else
            lo = mid;
    }
    /* lo is the desired value */
    return (seq_t)lo;
}
