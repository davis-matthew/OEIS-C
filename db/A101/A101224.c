/*
 * A101224  Triangle, read by rows, where T(n,1) = n^2-n+1 for n>=1 and T(n,k)
 *          = (n-k+1)*floor( (T(n,k-1)-1)/(n-k+1) ) for 1<k<=n; related to the
 *          Flavius sieve (A000960).
 * https://oeis.org/A101224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,k) where - m = smallest integer with m*(m+1)/2 >= n -
 *            k = n - (m-1)*m/2 (1 <= k <= m) - T(m,1) = m^2 - m + 1 - For i =
 *            2..k: T(m,i) = (m-i+1) * floor((T(m,i-1)-1)/(m-i+1))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Assumes seq_t and idx_t are defined elsewhere */
seq_t a(idx_t n) {
    /* Find the row m such that S(m) = m*(m+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 s = (unsigned __int128)mid * (mid + 1) / 2;
        if (s >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t m = low;
    /* k = index within the row */
    unsigned __int128 prev_s = (unsigned __int128)(m - 1) * m / 2;
    idx_t k = n - (idx_t)prev_s;  /* 1-based column */

    /* Compute T(m,k) */
    seq_t cur = (seq_t)m * (seq_t)m - (seq_t)m + 1;  /* T(m,1) */
    for (idx_t i = 2; i <= k; ++i) {
        seq_t denom = (seq_t)(m - i + 1);
        seq_t t = (cur - 1) / denom;
        cur = denom * t;
    }
    return cur;
}
