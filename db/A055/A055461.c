/*
 * A055461  Square decrescendo subsequences: triangle T(n,k) = (n-k)^2, n >=
 *          1, 0 <= k < n.
 * https://oeis.org/A055461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) equals (r – pos)² where r is the minimal integer such that
 *            * r(r + 1)/2 ≥ n and pos = n − r(r – 1)/2 – 1 (0‑based position
 *            in the r‑th row * of the triangle T(n,k) = (n – k)² ). * Thus
 *            a(n) is the n‑th element when the rows of the triangle are read
 *            * left‑to‑right and top‑to‑bottom
 * Range:     u8 n <= 120, u16 n <= 8680, u32 n <= 8680, u64 n <= 8680,
 *            u128 n <= 8680, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 128‑bit integer  (floor) */
static inline uint64_t isqrt128(unsigned __int128 x)
{
    uint64_t low = 0, high = ~ (uint64_t)0, mid, ans = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* prevent underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0)        /* the sequence is defined for n >= 1, but guard anyway */
        return 0;

    /* compute r: minimal integer with r(r+1)/2 >= n */
    unsigned __int128 discr = (unsigned __int128)8 * n + 1;
    uint64_t d = isqrt128(discr);
    idx_t r = (idx_t)((d - 1) / 2);

    unsigned __int128 cum = (unsigned __int128)r * (r + 1) / 2;
    while (cum < n) {
        ++r;
        cum = (unsigned __int128)r * (r + 1) / 2;
    }

    /* previous cumulative sum */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;

    idx_t pos = (idx_t)(n - prev - 1);      /* zero‑based position in row r   */
    idx_t diff = r - pos;                   /* r - k, where k = pos           */

    return (seq_t)diff * (seq_t)diff;
}
