/*
 * A134575  Triangle read by rows: T(n,k) = k^2*A051731(n,k).
 * https://oeis.org/A134575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k^2) if k divides m, 0 otherwise, where * m = smallest
 *            integer such that m*(m+1)/2 >= n, * k = n - (m-1)*m/2 (1‑based
 *            position in row m)
 * Range:     u8 n <= 135, u16 n <= 1275, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   prev_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/prev_tri.h"

static idx_t row_for_idx(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    idx_t r = row_for_idx(n);
    if (r == 0) return 0;

    idx_t prev = prev_tri(r);
    idx_t k = n - prev;          /* 1‑based position in row r */

    if (k == 0) return 0;        /* safety, should not happen for n>=1 */

    if (r % k == 0) {
        seq_t val = (seq_t)k * (seq_t)k;
        return val;
    }
    return 0;
}
