/*
 * A065916  Denominator of sigma(8*n^2)/sigma(4*n^2).
 * https://oeis.org/A065916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 2^(2*v2(n)+3) - 1, where v2(n) is the exponent of the
 *            largest power of 2 dividing n
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t count_trailing_zeros(idx_t x)
{
    /* Count factors of 2 in x.  x ≥ 1. */
    idx_t cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        ++cnt;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    idx_t t = count_trailing_zeros(n);
    idx_t exp = 2 * t + 3;          /* required binary exponent      */
    seq_t val = 1;                  /* start with 2^0 in seq_t domain */
    for (idx_t i = 0; i < exp; ++i)
        val <<= 1;                  /* multiply by 2 exp times      */
    val -= 1;                       /* 2^exp - 1                    */
    return val;
}
