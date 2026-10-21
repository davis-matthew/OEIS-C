/*
 * A214929  A209293 as table read layer by layer - layer clockwise, layer
 *          counterclockwise and so on.
 * https://oeis.org/A214929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = (m1 + m2 - 1)*(m1 + m2 - 2)/2 + m1 where t =
 *            floor(sqrt(n-1)) + 1 diff1 = n - (t-1)^2 diff2 = t^2 - n + 1
 *            min1 = min(t, diff1) min2 = min(t, diff2) if t odd: i = min1, j
 *            = min2 else: i = min2, j = min1 m1 = floor((i+j)/2) +
 *            floor(i/2)*(-1)^(2*i+j-1) m2 = floor((i+j+1)/2) +
 *            floor(i/2)*(-1)^...
 * Range:     u8 n <= 132, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include necessary headers */
#include <stdint.h>

/* If the helper _floor_isqrt_u64 is available, use it. */
/* Otherwise, implement a simple integer square root. */
#if defined(_floor_isqrt_u64)
static idx_t int_sqrt(idx_t n) { return _floor_isqrt_u64(n); }
#else
/* Simple integer sqrt for 64-bit unsigned integers. */
static idx_t int_sqrt(idx_t n) {
    idx_t x = 0;
    idx_t y = (1ULL << 32) - 1;  /* Upper bound for sqrt of 64-bit */
    while (x <= y) {
        idx_t mid = (x + y) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            x = mid + 1;
        } else {
            if (mid == 0) break;
            y = mid - 1;
        }
    }
    return x - 1;
}
#endif

/* Main sequence function */
seq_t a(idx_t n) {
    /* Step 1: determine layer t */
    idx_t t = int_sqrt(n - 1) + 1;        /* t = ceil(sqrt(n)) */

    /* Step 2: compute min1 and min2 */
    idx_t diff1 = n - (t - 1) * (t - 1);
    idx_t min1  = (diff1 < t) ? diff1 : t;

    idx_t diff2 = t * t - n + 1;
    idx_t min2  = (diff2 < t) ? diff2 : t;

    /* Step 3: compute i and j depending on parity of t */
    idx_t i, j;
    if (t & 1ULL) {      /* odd t */
        i = min1;
        j = min2;
    } else {             /* even t */
        i = min2;
        j = min1;
    }

    /* Step 4: compute m1 and m2 */
    /* Use signed integer for intermediate to handle subtraction */
    idx_t floor_i2 = i >> 1;          /* floor(i/2) */
    long long m1_calc = (long long)((i + j) >> 1)
                        + (((2 * i + j - 1) & 1ULL) ? -(long long)floor_i2
                                                    :  (long long)floor_i2);

    long long m2_calc = (long long)((i + j + 1) >> 1)
                        + (((2 * i + j - 2) & 1ULL) ? -(long long)floor_i2
                                                    :  (long long)floor_i2);

    idx_t m1 = (idx_t)m1_calc;
    idx_t m2 = (idx_t)m2_calc;

    /* Step 5: compute final result */
    idx_t s = m1 + m2 - 1;              /* s = m1 + m2 - 1 */
    seq_t result = ((seq_t)s * ((seq_t)s - 1ULL)) / 2ULL + (seq_t)m1;

    return result;
}
