/*
 * A207260  Triangle read by rows: T(n,k) = k^2 + (1-(-1)^(n-k))/2.
 * https://oeis.org/A207260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = k^2 + ((r - k) & 1) where r = ⌊(√(8n+1) – 1)/2⌋ and k = n
 *            – r(r+1)/2
 * Range:     u8 n <= 151, u16 n <= 9596, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square‑root of a 128‑bit unsigned integer */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = 1;
    while (high * high <= x)
        high <<= 1;
    while (low <= high) {
        unsigned __int128 mid   = (low + high) >> 1;
        unsigned __int128 midsq = mid * mid;
        if (midsq == x)
            return (idx_t)mid;
        if (midsq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return (idx_t)(low - 1);
}

seq_t a(idx_t n)
{
    /* D = 8n + 1, computed in 128‑bit to avoid overflow */
    unsigned __int128 D = (unsigned __int128)8 * n + 1;
    idx_t r = (isqrt_u128(D) - 1) >> 1;                 /* r = floor((sqrt(D)-1)/2) */
    idx_t tri = r * (r + 1) >> 1;                      /* triangular number r(r+1)/2 */
    idx_t k = n - tri;                                 /* column index */

    seq_t ksqr = (seq_t)k * (seq_t)k;                   /* k² in seq_t */
    seq_t parity = (seq_t)((r - k) & 1);                /* 1 if (r−k) odd, else 0 */

    return ksqr + parity;
}
