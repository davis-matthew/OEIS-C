/*
 * A156062  Riordan array (1/(1-x^4), x/(1-x^4)).
 * https://oeis.org/A156062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,c) where * r = floor( (sqrt(8n+1)-1)/2 ), * c = n -
 *            r(r+1)/2, * T(n,k) = 0 if (n-k) % 4 != 0, * = binom( (n+3k)/4 ,
 *            k ) otherwise. * This implements the flattened row‑wise
 *            enumeration of the * triangle defined in OEIS A156062
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc. */
#include <stddef.h>   /* for size_t */

static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow when n is large */
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) ++r;
    /* Compute column c in that row */
    idx_t c = n - (unsigned __int128)r * (r + 1) / 2;
    /* Check the parity condition (n-k) % 4 == 0 */
    if ((r - c) % 4 != 0) return (seq_t)0;
    /* Compute the binomial coefficient */
    idx_t m = (r + 3 * c) / 4;
    return _binom(m, c);
}
