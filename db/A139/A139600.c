/*
 * A139600  Square array T(n,k) = n*(k-1)*k/2+k, of nonnegative numbers
 *          together with polygonal numbers, read by antidiagonals upwards.
 * https://oeis.org/A139600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   For index n, let s = floor((sqrt(1+8n)-1)/2) and p = n -
 *            s*(s+1)/2; then a(n) = (s-p)*(p-1)*p/2 + p. (This uses the
 *            square array T(r,k) = r*(k-1)*k/2 + k read by antidiagonals
 *            upwards.)
 * Range:     u8 n <= 144, u16 n <= 4812, u32 n <= 9336, u64 n <= 9336,
 *            u128 n <= 9336, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find diagonal index s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t low = 0;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= (unsigned __int128)n)
        high <<= 1;
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 mids = (unsigned __int128)mid * (mid + 1) / 2;
        if (mids <= (unsigned __int128)n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t s = low - 1; /* diagonal index */

    unsigned __int128 start128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t p = n - start;     /* 0‑based position in diagonal */

    idx_t r = s - p;         /* row index */
    idx_t k = p;             /* column index */

    if (k == 0)
        return (seq_t)0;

    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t tmp = kk * (kk - 1) / 2;  /* (k-1)*k/2  */
    return rr * tmp + kk;
}
