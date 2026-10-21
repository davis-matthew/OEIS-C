/*
 * A204166  Symmetric matrix based on f(i,j)=ceiling[(i+j)/2], by
 *          antidiagonals.
 * https://oeis.org/A204166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ceil( ceil( (1 + sqrt(1 + 8 n)) / 2 ) / 2 )
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  n is 1‑based (a(1) = 1).  The sequence counts entries of the
    symmetric matrix f(i,j) = ceil((i + j)/2) by antidiagonals.  */

static unsigned __int128
isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 lo = 0, hi = ((unsigned __int128)1 << 64) - 1;
    while (lo <= hi) {
        unsigned __int128 mid = (lo + hi) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                    /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; behavior below 1 is irrelevant */
        return (seq_t)0;

    /* Compute sqrt(1 + 8 n) with integer arithmetic */
    unsigned __int128 tmp = (unsigned __int128)n * 8 + 1;
    unsigned __int128 s_floor = isqrt_u128(tmp);
    unsigned __int128 ceil_sqrt = s_floor;
    if (s_floor * s_floor < tmp)
        ++ceil_sqrt;          /* ceil(sqrt(tmp)) */

    /* k = ceil( (1 + sqrt(1+8n))/2 )  */
    unsigned __int128 k = (ceil_sqrt + 2) >> 1;      /* (ceil_sqrt + 2)/2 */

    /* a = ceil(k/2) = (k + 1)/2 */
    unsigned __int128 res = (k + 1) >> 1;            /* (k + 1)/2 */

    return (seq_t)res;
}
