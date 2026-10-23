/*
 * A026309  a(n) = n-th nonnegative integer k satisfying sin(k) < sin(k+1).
 * https://oeis.org/A026309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th nonnegative integer k satisfying sin(k) < sin(k+1)
 * Range:     u8 n <= 127, u16 n <= 4000, u32 n <= 4000, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The criterion is equivalent to cos(k+0.5) > 0, i.e. k+0.5 lies in
 * the union of intervals (2πm - π/2, 2πm + π/2) for m = 0,1,2,...  */
/* We use the rational convergent 104348/33215 ≈ π, which is accurate to
 * 10⁻¹⁰.  The resulting linear bounds are expressed exactly as integers
 * and then converted to floor / ceil with integer arithmetic. */

#include <stdint.h>

static inline int64_t floor_div(int64_t a, int64_t b)
{
    /* floor(a / b) for signed integers, b > 0 */
    if (a >= 0) return a / b;
    /* a < 0: floor towards -∞ */
    return -(((-a) + b - 1) / b);
}

static inline int64_t ceil_div(int64_t a, int64_t b)
{
    /* ceil(a / b) for signed integers, b > 0 */
    if (a >= 0) return (a + b - 1) / b;
    /* a < 0: truncation towards zero already gives the ceil value */
    return a / b;
}

seq_t a(idx_t n)
{
    /* target is 1‑based index in the sequence */
    idx_t target = n;
    idx_t count  = 0;          /* how many acceptable k’s seen so far */
    idx_t m      = 0;          /* interval counter */

    /* Rational approximation of π : 104348 / 33215 */
    const int64_t p = 104348;       /* numerator of π  */
    const int64_t q = 33215;        /* denominator of π  */
    const int64_t denom = 2 * q;    /* 2q in the formulas below */

    while (1)
    {
        /* Lower numerator and upper numerator of the bounds in common denominator 2q */
        int64_t low_num  = 4 * p * (int64_t)m - p - q;   /* (4p m - p - q) / (2q) */
        int64_t high_num = 4 * p * (int64_t)m + p - q;   /* (4p m + p - q) / (2q) */

        /* floor for lower bound, ceil for upper bound */
        int64_t low_floor = floor_div(low_num, denom);    /* floor(L_m) */
        int64_t low_int   = low_floor + 1;                /* smallest integer > L_m */

        /* Ensure the lower bound is not negative; we ignore k < 0 */
        if (low_int < 0) low_int = 0;

        int64_t high_ceil = ceil_div(high_num, denom);    /* ceil(U_m) */
        int64_t high_int  = high_ceil - 1;                /* largest integer < U_m */

        if (high_int < 0) {                /* interval entirely negative */
            ++m;
            continue;
        }

        if (low_int <= high_int) {
            idx_t seg_count = (idx_t)(high_int - low_int + 1);

            if (count + seg_count >= target) {
                /* Desired term is within this block */
                return (seq_t)(low_int + (target - count - 1));
            }
            count += seg_count;
        }
        ++m;
    }
}
