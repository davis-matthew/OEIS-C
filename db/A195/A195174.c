/*
 * A195174  a(n) = 5*n - floor(3*n*r), where r=(1+sqrt(5))/2 (the golden
 *          ratio.)
 * https://oeis.org/A195174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n - floor(3*n*φ) , φ = (1+sqrt5)/2
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 1793, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor of sqrt of an arbitrary-precision integer.
   Uses binary search on [0, hi], where hi is a power of two
   large enough that hi*hi > x. */
static seq_t
sqrt_floor(seq_t x)
{
    seq_t low = 0, high = 1;
    /* grow high until high*high > x */
    while (high * high <= x) {
        high <<= 1;
    }
    /* binary search between low and high-1 */
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1; /* upper mid */
        if (mid * mid <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* a = 3*n (in arbitrary precision) */
    seq_t a = (seq_t) n * (seq_t)3;

    /* t = floor(3*n*sqrt5) = floor( sqrt(5) * (3*n) )
       Compute via integer sqrt of 5 * a * a */
    seq_t t = sqrt_floor(((seq_t)5) * a * a);

    /* floor(3*n*φ) = floor( (a + t) / 2 ) */
    seq_t floor_val = (a + t) >> 1; /* integer division by 2 */

    /* a(n) = 5*n - floor_val */
    seq_t res = (seq_t) n * (seq_t)5 - floor_val;

    return res;
}
