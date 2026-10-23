/*
 * A127241  A Thue-Morse triangle.
 * https://oeis.org/A127241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = parity of the popcount of C(c, r‑c), where r = max{ m :
 *            m*(m+1)/2 ≤ n } and c = n – r*(r+1)/2
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom_parity_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_parity_popcount.h"
/* Compute the row index r for a given longitudinal index n. */
static idx_t tri_row(idx_t n) {
    idx_t low = 0, high = 1;
    /* grow high so that its triangular number exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    /* binary search for the greatest r with T(r) <= n */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t r = tri_row(n);
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - start;
    if (r > 2 * c)          /* binomial is zero → parity of popcount = 0 */
        return (seq_t)0;
    return binom_parity_popcount(c, r - c);
}
