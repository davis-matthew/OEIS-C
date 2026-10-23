/*
 * A180446  Number of non-pentagonal numbers <= n.
 * https://oeis.org/A180446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n - floor((sqrt(24*n+1)+1)/6)
 * Range:     u8 n <= 268, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the floor square root of an unsigned __int128 via binary search */
static inline unsigned long long
isqrt_u128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = 0xFFFFFFFFFFFFFFFFULL;
    while (lo <= hi) {
        unsigned long long mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == x) return mid;
        if (sq < x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;
}

/* Number of non‑pentagonal numbers <= n */
seq_t
a(idx_t n)
{
    /* compute 24*n + 1 using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 prod = (unsigned __int128)n * 24ULL + 1ULL;
    unsigned long long sqrt_val = isqrt_u128(prod);          /* floor sqrt(24*n+1) */
    unsigned long long t = (sqrt_val + 1ULL) / 6ULL;         /* floor((sqrt+1)/6)   */
    /* result fits in seq_t since n <= t <= n, subtraction keeps non‑negative */
    return (seq_t)(n - t);
}
