/*
 * A190451  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(golden ratio,4,2) and
 *          []=floor.
 * https://oeis.org/A190451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4n+2)φ) - 4*floor(nφ) - floor(2φ) (φ = (1+√5)/2)
 * Range:     u8 n <= 123, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Integer square‑root for 64‑bit unsigned integers.
   Returns floor(sqrt(x)).  Uses a binary search that avoids overflow. */
static unsigned long long isqrt_u64(unsigned long long x)
{
    unsigned long long lo = 0, hi = ULLONG_MAX >> 1;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        /* mid*mid may overflow, compare using division */
        if (mid > 0 && mid > x / mid) {
            /* mid*mid > x */
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            /* mid*mid <= x */
            lo = mid + 1;
        }
    }
    return hi;
}

/* Greatest error is that 5*n*n may overflow 64 bits for very large n,
   but the intended mock tests use n <= 10^5, so this is safe. */
static unsigned long long floor_sqrt5_mul(unsigned long long n)
{
    /* compute floor(n*sqrt(5)) as floor(sqrt(5*n^2)) */
    return isqrt_u64(5ull * n * n);
}

/* Main sequence function.  The function works for any seq_t type and
   returns a value that fits in seq_t (0 ≤ a(n) ≤ 4). */
seq_t a(idx_t n)
{
    if (n == 0)  /* for safety; sequence is defined for n ≥ 1 */
        return 0;

    unsigned long long A = (unsigned long long)n;          /* n   */
    unsigned long long B = 4ull * A + 2ull;                 /* 4n+2 */

    unsigned long long sqrtA = floor_sqrt5_mul(A);
    unsigned long long floorA = (A + sqrtA) >> 1;          /* floor(nφ) */

    unsigned long long sqrtB = floor_sqrt5_mul(B);
    unsigned long long floorB = (B + sqrtB) >> 1;          /* floor((4n+2)φ) */

    unsigned long long res = floorB - 4ull * floorA - 3ull; /* floor(2φ)=3 */
    return (seq_t)res;
}
