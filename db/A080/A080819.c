/*
 * A080819  Row sums from triangle in A080818.
 * https://oeis.org/A080819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (ceiling(sqrt(n*(n+1)/2)))^2
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the floor of the square‑root of a seq_t value without overflow */
static seq_t floor_isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = 1;
    /* grow high until high^2 > x, using division to avoid overflow */
    while (high <= x / high) {
        high <<= 1;
    }
    low = high >> 1;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        if (mid <= x / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    /* n ≥ 1 by specification; behaviour for n < 1 is irrelevant */
    /* t = n*(n+1)/2, computed in seq_t to avoid overflow */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* floor of sqrt(t) */
    seq_t fsqrt = floor_isqrt(t);

    /* ceil of sqrt(t) */
    seq_t ceil_sqrt;
    if (fsqrt * fsqrt == t) {
        ceil_sqrt = fsqrt;
    } else {
        ceil_sqrt = fsqrt + 1;
    }

    /* return ceil_sqrt^2 */
    return ceil_sqrt * ceil_sqrt;
}
