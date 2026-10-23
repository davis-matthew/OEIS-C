/*
 * A106314  Triangle T(n,k) composed of the squares min(n,k)^2.
 * https://oeis.org/A106314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (min(k, r+1-k))^2 * where r is the smallest integer with
 *            r*(r+1)/2 >= n, * and k = n - r*(r-1)/2
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1, but keep safe */

    /* Find r by binary search on < 2^64 (fits in unsigned long long). */
    unsigned long long low  = 1;
    unsigned long long high = 1;
    while (1) {
        unsigned __int128 val = (unsigned __int128)high * (high + 1) / 2;
        if (val >= n) break;
        if (high > (1ULL << 63)) { high = (1ULL << 63); break; }
        high <<= 1;
    }

    while (low < high) {
        unsigned long long mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    unsigned long long r = low;

    /* previous count of elements before row r */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    unsigned long long prev = (unsigned long long)prev128;

    /* column index within the row (1‑based) */
    unsigned long long k = n - prev;

    /* The symmetric minimal index */
    unsigned long long m = r + 1 - k;
    unsigned long long s = (k < m ? k : m);

    /* Result: square of s */
    seq_t val = (seq_t)s * (seq_t)s;
    return val;
}
