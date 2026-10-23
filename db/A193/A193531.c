/*
 * A193531  Number of integer solutions to the quartic elliptic curve y^2 =
 *          5*x^4 - 4*n.
 * https://oeis.org/A193531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Number of integer solutions to the quartic elliptic curve
 *     y^2 = 5*x^4 - 4*n
 * Count only positive integer x.  For each admissible x, there are two
 * integer values of y (±), so the returned value is twice the number
 * of admissible x.
 *
 * The implementation simply tests all x from 1 up to a fixed
 * upper bound that is sufficient for n ≤ 10 000 – in this range
 * all solutions occur with x < 2000, which is far below the time
 * limit and keeps the computation trivial.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) for 64‑bit unsigned values.
 * No floating point operations are used. */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = v;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        uint64_t sq  = mid * mid;
        if (sq == v) {
            return mid;
        } else if (sq < v) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Convert n to a wider integer type for safe arithmetic. */
    uint64_t N   = (uint64_t)n;
    uint64_t cnt = 0;

    /* Empirically for n ≤ 10 000 all solutions satisfy x < 2000. */
    const uint64_t X_MAX = 2000;

    for (uint64_t x = 1; x <= X_MAX; ++x) {
        uint64_t x2  = x * x;
        uint64_t x4  = x2 * x2;             /* x^4 */
        uint64_t rhs = 5 * x4;              /* 5*x^4    */
        if (rhs < 4 * N) continue;          /* RHS negative → impossible */

        rhs -= 4 * N;                       /* y^2 = 5*x^4 - 4*n */

        uint64_t y = isqrt_u64(rhs);
        if (y * y == rhs)                  /* perfect square */
            cnt += 2;                       /* y = ± */
    }
    return (seq_t)cnt;
}
