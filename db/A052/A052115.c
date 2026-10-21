/*
 * A052115  Number of nonnegative integer pairs (i,j) with binomial(i+r,r) +
 *          binomial(j+r,r) <= binomial(n+r,r), r=2.
 * https://oeis.org/A052115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with
 *            binomial(i+2,2)+binomial(j+2,2) <= binomial(n+2,2)
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer sqrt for unsigned 64‑bit values (floor of sqrt) */
static inline uint64_t isqrt_uint64(uint64_t v)
{
    uint64_t low = 0, high = v >> 1 + 1; /* upper bound */
    while (low <= high) {
        uint64_t mid = low + (high - low) / 2;
        uint64_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* no admissible pairs */
    uint64_t fn = (uint64_t)(n + 2) * (uint64_t)(n + 1);   /* f(n) */
    seq_t total = 0;
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t fi = (uint64_t)(i + 2) * (uint64_t)(i + 1); /* f(i) */
        if (fi > fn) break;                                 /* beyond range */
        uint64_t T = fn - fi;                               /* remaining budget */
        if (T == 0) continue;                              /* f(j) cannot be 0 */
        uint64_t val  = 1 + 4 * T;                          /* 1 + 4*T */
        uint64_t sqv  = isqrt_uint64(val);                  /* floor sqrt */
        if (sqv <= 2) continue;                            /* no j satisfy */
        uint64_t hi = (sqv - 3) / 2;                       /* maximal j */
        total += (seq_t)(hi + 1);                           /* count j=0..hi */
    }
    return total;
}
