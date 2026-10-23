/*
 * A116588  Array read by antidiagonals: T(n,k) = max(2^(n - k), 2^(k - n)).
 * https://oeis.org/A116588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) reads the infinite Toeplitz matrix T(i,j)=2^{|i-j|} by
 *            antidiagonals (i.e., by constant i+j). The sequence is a(0)=1,
 *            a(1)=2, a(2)=2, a(3)=4, …
 * Range:     u8 n <= 35, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s such that the cumulative count
       up to and including that diagonal exceeds n.  The
       number of terms up to diagonal s is (s+1)(s+2)/2. */
    idx_t low = 0, high = 1;
    while (1) {
        __int128 prod = (__int128)(high + 1) * (high + 2) / 2;
        if (prod > n) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 prod = (__int128)(mid + 1) * (mid + 2) / 2;
        if (prod > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                     /* diagonal index   */
    idx_t t = n - (__int128)s * (s + 1) / 2;   /* offset on the diagonal */

    /* The term value is 2^{|i-j|} where i=t, j=s-t.  Hence exponent
       e = |2t - s|. */
    idx_t diff = 2 * t > s ? 2 * t - s : s - 2 * t;
    return pow2(diff);
}
