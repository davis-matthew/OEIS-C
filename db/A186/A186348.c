/*
 * A186348  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) after
 *          g(j) when f(i)=g(j), where f(i)=8i and g(j)=j^2. Complement of
 *          A186349.
 * https://oeis.org/A186348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(8*n))
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt128(__int128 x)
{
    /* binary search for floor(sqrt(x)) when x <= 8*UINT64_MAX */
    idx_t low = 0;
    idx_t high = (idx_t)1 << 34; /* 2^34 > sqrt(8*2^64) */

    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

seq_t a(idx_t n)
{
    /* 8*n may overflow 64 bits; use 128‑bit for the product */
    __int128 prod = (__int128)8 * (__int128)n;
    idx_t s = isqrt128(prod);              /* floor(sqrt(8*n)) */
    return (seq_t)(n + s);                  /* n + floor(sqrt(8*n)) */
}
