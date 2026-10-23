/*
 * A162614  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n, such that the difference between successive terms is equal
 *          to n^3 - 1.
 * https://oeis.org/A162614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   For row r (0‑indexed) the terms are * T(r,k) = r + k·(r³ – 1), k
 *            = 0…r. * The sequence a(n) is the triangle read by rows, i.e. *
 *            if r is the smallest index with (r+1)(r+2)/2 > n, * let k = n –
 *            r(r+1)/2; then a(n) = T(r,k)
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

static inline unsigned long long
isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = 1;
    /* grow hi until hi² > x */
    while ((unsigned __int128)hi * hi <= x) {
        if (hi == ULLONG_MAX) break;        /* cannot go higher */
        hi <<= 1;
    }
    /* binary search between lo and hi (hi is exclusive) */
    while (lo + 1 < hi) {
        unsigned long long mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Special case for the first row */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r such that (r+1)(r+2)/2 > n */
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    unsigned long long s = isqrt128(val);          /* floor(sqrt(8n+1)) */
    unsigned long long r;
    if (s < 3)
        r = 0;
    else
        r = (s - 3) / 2 + 1;                      /* smallest r with S(r)>n */

    /* Number of terms before row r */
    unsigned __int128 terms_before = ((unsigned __int128)r * (r + 1)) / 2;
    unsigned long long k = (unsigned long long)(n - (idx_t)terms_before);

    /* Compute T(r,k) = r + k*(r³-1) */
    seq_t rn = (seq_t)r;
    seq_t diff = rn * rn * rn;      /* r³  */
    diff -= 1;                       /* r³ – 1 */
    seq_t kn = (seq_t)k;
    return rn + diff * kn;
}
