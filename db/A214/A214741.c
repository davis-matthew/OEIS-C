/*
 * A214741  Least m>0 such that n-m divides L(n)+m, where L = A000201 (lower
 *          Wythoff sequence).
 * https://oeis.org/A214741
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m>0 such that (n‑m) divides (L(n)+m), where L(n) =
 *            floor(n*phi) and phi = (1+sqrt(5))/2 (lower Wythoff sequence)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 93 terms through n = 996 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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

static seq_t
lower_wythoff(idx_t n)
{
    /* Compute floor(n*phi) using integer arithmetic:
       floor(n*phi) = floor( (n + floor(sqrt(5*n^2))) / 2 )   */
    seq_t ns = (seq_t)n;          /* n as seq_t (for multiplications) */
    seq_t sq = ns * ns;           /* n^2   */
    seq_t nsq5 = sq * (seq_t)5;   /* 5*n^2 */
    seq_t t = floor_sqrt(nsq5);   /* floor(sqrt(5*n^2)) */
    return ( (seq_t)n + t ) / 2;  /* floor((n + t)/2) */
}

seq_t a(idx_t n)
{
    if (n < 2)            /* offset is 2, values for n<2 are undefined */
        return 0;

    seq_t L = lower_wythoff(n);
    for (idx_t m = 1; m < n; ++m) {
        idx_t d = n - m;                     /* divisor (n-m) > 0 */
        seq_t val = L + (seq_t)m;            /* L + m   */
        seq_t dd = (seq_t)d;                 /* divisor as seq_t */
        if (val % dd == 0)
            return (seq_t)m;                 /* first such m is the answer */
    }
    return 0;   /* should never occur for valid n */
}
