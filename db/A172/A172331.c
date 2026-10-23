/*
 * A172331  a(n) = floor(n*(sqrt(13)+sqrt(7))).
 * https://oeis.org/A172331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = floor(n*(sqrt(13)+sqrt(7)))
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Small integer square root for 64 bit unsigned values.
   Returns floor(sqrt(x)). */
static inline uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = x, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        uint64_t sq = mid * mid;
        if (sq == x)
            return mid;
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Convert index to 64‑bit integer for safe intermediate
       calculations.  idx_t is at least 64‑bit. */
    uint64_t N   = (uint64_t)n;
    uint64_t n2  = N * N;           /* n^2  */
    uint64_t n4  = n2 * n2;         /* n^4  */

    /* floor(n*sqrt13) = floor(sqrt(13*n^2)) */
    uint64_t s13 = isqrt64(13ULL * n2);
    /* floor(n*sqrt7)  = floor(sqrt(7*n^2))  */
    uint64_t s7  = isqrt64(7ULL  * n2);

    uint64_t sum  = s13 + s7;       /* floor(n*sqrt13)+floor(n*sqrt7)   */
    uint64_t cand = sum + 1;        /* candidate value = sum+1            */

    /* Check whether cand <= n*(sqrt13+sqrt7).  Use the equivalent
       inequality (cand^2 <= 20*n^2 + 2*sqrt91*n^2), which after
       squaring becomes (cand^2-20*n^2)^2 <= 364*n^4.  All terms fit
       in 64 bits for the specified n <= 10000.                    */
    uint64_t X  = cand * cand;                  /* (sum+1)^2                    */
    uint64_t L  = X - 20ULL * n2;               /* X - 20*n^2                    */
    uint64_t RHS = 364ULL * n4;                 /* 364*n^4                        */
    uint64_t LL  = L * L;                       /* L^2                           */

    if (LL <= RHS)
        return (seq_t)cand;
    else
        return (seq_t)sum;
}
