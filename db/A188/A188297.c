/*
 * A188297  a(n) = [n*r] - [k*r] - [n*r-k*r], where r=1/sqrt(2), k=2, [
 *          ]=floor.
 * https://oeis.org/A188297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*r) - floor((n-2)*r) - 1, r = 1/sqrt(2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 145 terms through n = 9402 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Helper: floor(m * r) for m >= 0 (r = 1/sqrt(2)).  */
static seq_t floor_div_sqrt2(idx_t m)
{
    /*  floor(m/√2) = floor( floor(m√2) / 2 ).
        floor(m√2) = floor( sqrt(2 * m²) ) – compute with a binary search.  */
    __uint128_t tmp = (__uint128_t)m * m * 2;      /* 2 * m²   */
    uint64_t lo = 0;
    uint64_t hi = 1;
    /* Find an upper bound for the integer square root of tmp.  */
    while (( (__uint128_t)hi * hi) <= tmp) {
        hi <<= 1;
    }
    uint64_t sqrt_val = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t mid2 = (__uint128_t)mid * mid;
        if (mid2 <= tmp) {
            sqrt_val = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    /* floor( floor(m√2) / 2 ) = sqrt_val >> 1  */
    return (seq_t)(sqrt_val >> 1);
}

seq_t a(idx_t n)
{
    /*  n = 1,2 give the value 0 directly.  */
    if (n < 3)
        return 0;

    seq_t f_n  = floor_div_sqrt2(n);
    seq_t f_n2 = floor_div_sqrt2(n - 2);
    seq_t diff = f_n - f_n2;          /* 1 or 2 for n ≥ 3 */
    return diff - 1;                  /* 0 or 1 */
}
