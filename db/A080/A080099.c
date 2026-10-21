/*
 * A080099  Triangle T(n,k) = n AND k, 0<=k<=n, bitwise logical AND, read by
 *          rows.
 * https://oeis.org/A080099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, hear, look
 * Formula:   Let r be the smallest integer such that (r+1)(r+2)/2 > n. Let k
 *            = n - r*(r+1)/2. Then a(n) = r & k. The sequence is the
 *            flattening of the triangle T(n,k)=n AND k
 * Range:     u8 n <= 9333, u16 n <= 9333, u32 n <= 9333, u64 n <= 9333,
 *            u128 n <= 9333, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t N = n;              /* make a copy of the index */
    idx_t low = 0, high = 1;

    /* enlarge high until pref(high) > N, where pref(r) = (r+1)(r+2)/2 */
    while ((unsigned __int128)(high + 1) * (high + 2) / 2 <= N) {
        if (high > (UINT64_MAX >> 1))
            break;           /* prevent overflow when doubling */
        high <<= 1;
    }

    /* binary search for the minimal r with pref(r) > N */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __uint128_t val = (__uint128_t)(mid + 1) * (mid + 2) / 2;
        if (val > N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;   /* r is the row index */

    /* prefix sum up to the previous row: pref(r-1) = r*(r+1)/2 */
    __uint128_t pref_before = (__uint128_t)r * (r + 1) / 2;
    idx_t k = N - (idx_t)pref_before;   /* column index within the row */

    return ((seq_t)r) & ((seq_t)k);
}
