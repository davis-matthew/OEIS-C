/*
 * A083035  a(n) = floor(sqrt(2)*n)-2*floor(n/sqrt(2)).
 * https://oeis.org/A083035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)) mod 2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute integer square root of a 128‑bit unsigned integer */
static unsigned __int128 isqrt_u128(unsigned __int128 v)
{
    unsigned __int128 low = 0, high = ((unsigned __int128)1) << 64; /* 2^64 is > sqrt(max 128‑bit) */
    unsigned __int128 ans = 0;
    while (low <= high)
    {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq = mid * mid;
        if (sq <= v)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0)
                break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0u)        /* defined only for n >= 1, but guard for safety */
        return (seq_t)0;
    unsigned __int128 nn = n;            /* promote to 128‑bit to avoid 64‑bit overflow */
    unsigned __int128 val = nn * nn * (unsigned __int128)2;  /* val = 2 * n^2 */
    unsigned __int128 s   = isqrt_u128(val);                 /* s = floor( sqrt(2*n^2) ) = floor(n*sqrt(2)) */
    return (seq_t)(s & 1u);   /* return parity (0 or 1) */
}
