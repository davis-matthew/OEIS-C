/*
 * A187969  a(n) = [nr+kr]-[nr]-[kr], where r=sqrt(2), k=3, [ ]=floor.
 * https://oeis.org/A187969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A187969
   a(n) = floor((n+3)*sqrt(2)) – floor(n*sqrt(2)) – 4
   (offset 1: a(1) is the first listed term)
   Note: floor(n*sqrt(2)) equals floor(sqrt(2*n^2)). */

#include <stdint.h>

/* Integer square root: returns floor(sqrt(x)) for 0 <= x <= UINT64_MAX.
   Algorithm uses binary search without overflow. */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t low  = 0;
    uint64_t high = (x < 1ULL<<32) ? x : (1ULL<<32);   /* upper bound: sqrt(UINT64_MAX) < 2^32 */
    uint64_t res  = 0;

    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* avoid division by zero in next test */
            res = 0;
            low = mid + 1;
            continue;
        }
        if (mid <= x / mid) {    /* mid*mid <= x without overflow */
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break; /* safety for unsigned underflow */
            high = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used for offset 1, but defensive */

    uint64_t nn = (uint64_t)n;                     /* convert idx_t to uint64_t */
    uint64_t m1 = nn * nn * 2ULL;                  /* 2*n^2  */
    uint64_t m2 = (nn + 3ULL) * (nn + 3ULL) * 2ULL;/* 2*(n+3)^2 */

    uint64_t f1 = isqrt64(m1);   /* floor(n*sqrt(2)) */
    uint64_t f2 = isqrt64(m2);   /* floor((n+3)*sqrt(2)) */

    uint64_t diff = f2 - f1 - 4ULL;   /* result is 0 or 1 for all n >= 1 */

    return (seq_t)diff;
}
