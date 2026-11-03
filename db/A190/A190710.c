/*
 * A190710  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,3) and [ ]=floor.
 * https://oeis.org/A190710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊(4n+3)√3⌋ - 4⌊n√3⌋ - 5; √3 ≈ 1.732…
 * Range:     u8 n <= 132, u16 n <= 132, u32 n <= 132, u64 n <= 132,
 *            u128 n <= 132, big any n
 * Validated: partial, pass, 132 terms through n = 132 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer floor of k·√3:  floor(k*sqrt(3)) = floor(sqrt(3*k*k))   */
static uint64_t floor_sqrt3(uint64_t k)
{
    if (k == 0) return 0;

    /* target = 3*k*k  (fits in 64 bits for the tests used) */
    uint64_t target = 3ULL * k * k;

    /* High bound > floor(k√3).  Since √3 < 2, 2*k+2 is safe.        */
    uint64_t low  = 0;
    uint64_t high = 2ULL * k + 2;
    uint64_t res  = 0;

    while (low <= high) {
        uint64_t mid   = (low + high) >> 1;
        uint64_t mid2  = mid * mid;

        if (mid2 <= target) {
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
    uint64_t nn      = (uint64_t)n;
    uint64_t term1   = floor_sqrt3(4ULL * nn + 3ULL);  /* ⌊(4n+3)√3⌋ */
    uint64_t term2   = floor_sqrt3(nn);                 /* ⌊n√3⌋     */
    uint64_t result  = term1 - 4ULL * term2 - 5ULL;     /* a(n)       */
    return (seq_t)result;
}
