/*
 * A225465  Triangular array read by rows: T(n, k) is the number of rooted
 *          forests on {1, 2, ..., n} in which one tree has been specially
 *          designated that contain exactly k trees; n >= 1, 1 <= k <= n.
 * https://oeis.org/A225465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m, k) where m is minimal with m(m+1)/2 ≥ n and k = n −
 *            (m−1)m/2, T(m,k) = binomial(m−1, k−1) * m^(m−k) * k
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* safety for out‑of‑range calls */

    /* find the row r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while (((unsigned __int128)high * (high + 1)) / 2 < n)
    {
        high <<= 1;
        if (high > n) high = n;            /* never grow beyond n */
    }
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = ((unsigned __int128)mid * (mid + 1)) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = ((unsigned __int128)(r - 1) * r) / 2;
    idx_t k = n - prev;

    /* compute the required value */
    seq_t bin  = binomial((idx_t)(r - 1), (idx_t)(k - 1));
    seq_t powt = big_pow((seq_t)r, (seq_t)(r - k));
    seq_t res  = bin * powt;
    res *= (seq_t)k;
    return res;
}
