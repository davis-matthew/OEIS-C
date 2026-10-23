/*
 * A171152  Partial sums of A118011.
 * https://oeis.org/A171152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 + n + R * ((6*n + 1) - R^2) / 6, where R =
 *            round(sqrt(2*n))
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt_u128(__uint128_t v)
{
    /* Return the floor of the square root of v.  v ≤ 2^65, so the result
       is guaranteed to fit in idx_t (uint64_t). */
    idx_t low = 0;
    idx_t high = ((idx_t)1 << 33);          /* 2^33 > sqrt(2^65) */

    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

static inline idx_t round_sqrt2n(idx_t n)
{
    __uint128_t twice = ((__uint128_t)n) << 1;   /* 2*n, safe in 128 bits */

    /* Floor sqrt first. */
    idx_t floor_root = isqrt_u128(twice);

    /* Compute remainder to decide rounding. */
    __uint128_t floor_sq   = (__uint128_t)floor_root * floor_root;
    __uint128_t next_sq    = (__uint128_t)(floor_root + 1) * (floor_root + 1);
    __uint128_t diff_low  = twice - floor_sq;
    __uint128_t diff_high = next_sq - twice;

    /* round to nearest integer */
    if (diff_low > diff_high)
        return floor_root + 1;
    return floor_root;
}

seq_t a(idx_t n)
{
    /* Convert n to a 128‑bit integer for safe arithmetic. */
    __uint128_t N = n;

    /* Compute the rounding integer R. */
    idx_t r = round_sqrt2n(n);
    __uint128_t R = r;

    /* Intermediate:  R * ((6*N + 1) - R^2)  */
    __uint128_t part_num = R * ( (6 * N + 1) - R * R );

    /* Division by 6 is integer‑exact by the OEIS formula. */
    __uint128_t part = part_num / 6;

    /* Final result: N^2 + N + part. */
    __uint128_t result = N * N + N + part;

    return (seq_t)result;
}
