/*
 * A141611  Triangle read by rows: T(n, k) = (n-k+1)*(k+1)*binomial(n, k).
 * https://oeis.org/A141611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (r-k+1)*(k+1)*C(r,k) where r = floor((sqrt(8*n+1)-1)/2)
 *            and k = n - r(r+1)/2 C(r,k) is the binomial coefficient
 * Range:     u8 n <= 23, u16 n <= 95, u32 n <= 416, u64 n <= 1736,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binomial coefficient r choose k computed multiplicatively.
   Works for all supported seq_t types because intermediate
   results stay within the same type (overflow is expected only
   when the value does not fit in seq_t). */
static seq_t binom(idx_t r, idx_t k)
{
    if (k > r - k)
        k = r - k;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(r - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Helper: floor of triangular index: largest r with r(r+1)/2 <= n. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* Double high until triangular number exceeds n. */
    while ((__int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* Binary search between low and high. */
    while (low <= high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    seq_t val = binom(r, k);
    val *= (seq_t)(r - k + 1);
    val *= (seq_t)(k + 1);

    return val;
}
