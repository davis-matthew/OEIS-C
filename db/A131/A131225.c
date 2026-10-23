/*
 * A131225  Triangle read by rows: T(n,k) = 2*k - (1 + (-1)^(n-k))/2, for 1 <=
 *          k <= n, n >= 1.
 * https://oeis.org/A131225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th entry of the triangle T(n,k) = 2*k - (1 +
 *            (-1)^(n-k))/2 read by rows; for each n>=1, r = smallest s.t.
 *            s*(s+1)/2 >= n, k = n - (r-1)*r/2, and a(n) = 2*k - ((r-k) even
 *            ? 1 : 0)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t find_row(idx_t n) {
    idx_t low  = 1;
    idx_t high = 1;
    /* double until high*(high+1)/2 >= n */
    while ((__int128)high * (high + 1) / 2 < n) {
        if (high > (UINT64_MAX >> 1)) { high = UINT64_MAX; break; }
        high <<= 1;
    }
    /* binary search for the exact row */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* offset is 1, behaviour for n=0 is irrelevant */
    idx_t r = find_row(n);

    /* previous triangular number T(r-1) */
    __int128 prev = (__int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)prev;

    seq_t val = (seq_t)2 * k;
    if (((r - k) & 1ULL) == 0)      /* (r-k) even -> subtract 1 */
        val -= 1;
    return val;
}
