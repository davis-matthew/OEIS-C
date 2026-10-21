/*
 * A089913  Table T(n,k) = lcm(n,k)/gcd(n,k) = n*k/gcd(n,k)^2 read by
 *          antidiagonals (n >= 1, k >= 1).
 * https://oeis.org/A089913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = lcm(i,j)/gcd(i,j) where (i,j) are the coordinates of the
 *            n-th cell in the antidiagonal enumeration of the infinite table
 *            T(i,j) = i*j / gcd(i,j)^2 (i,j ≥ 1)
 * Range:     u8 n <= 508, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset unused */

    /* Find diagonal sum s such that (s‑1)*s/2 ≥ n. */
    idx_t low = 1, high = 2;
    while (1) {
        unsigned __int128 cum = (unsigned __int128)high * (high - 1) / 2;
        if (cum >= n) break;
        if (high > (UINT64_MAX >> 1)) { high = UINT64_MAX; break; }
        high <<= 1;
    }

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = (unsigned __int128)mid * (mid - 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;                     /* i + j */

    /* Elements before this diagonal. */
    unsigned __int128 prev_cum128 = (unsigned __int128)(s - 1) * (s - 2) / 2;
    idx_t prev = (idx_t)prev_cum128;

    idx_t p  = n - prev - 1;           /* zero‑based position on diagonal */
    idx_t i  = 1 + p;                  /* row index  (1 ≤ i ≤ s‑1) */
    idx_t j  = s - i;                  /* column index */

    /* Compute T(i,j) = i*j / gcd(i,j)^2 using division first. */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t g  = gcd(ii, jj);
    seq_t a  = ii / g;
    seq_t b  = jj / g;
    return a * b;
}
