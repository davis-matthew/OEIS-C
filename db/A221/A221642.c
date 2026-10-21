/*
 * A221642  Square array read by antidiagonals: T(n,k) in which if k = j^2
 *          then column k lists j interleaved with j-1 zeros. Otherwise if k =
 *          j^2 + j then column k lists the positive integers, starting with
 *          j+1, interleaved with j-1 zeros. Otherwise column k lists only
 *          zeros. n>=1, j>=1, k>=1.
 * https://oeis.org/A221642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   compute a(n) by reading the square array T(i,k) along
 *            anti‑diagonals. The array is defined by if k = j² → T(i,k) = j
 *            if (i‑1)%j==0 else 0 if k = j² + j → T(i,k) = (i‑1)/j + j + 1 if
 *            (i‑1)%j==0 else 0 otherwise → 0 The anti‑diagonal d (starting
 *            with d = 2) contains the entries (d‑1,1),(d‑2,2),…,(1...
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned value */
static inline idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 prod = (unsigned __int128)mid * mid;
        if (prod <= x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            hi  = mid - 1;
        }
    }
    return ans;
}

/* find the smallest diagonal d such that (d‑1)d/2 ≥ n.
   n ≥ 1.  The value d satisfies 2 ≤ d ≤ 2n, so the search range
   is safe for the given constraints. */
static inline idx_t find_diagonal(idx_t n)
{
    idx_t low  = 2;
    idx_t high = 2 * n;          /* high is always large enough           */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 cum = (unsigned __int128)(mid - 1) * mid / 2;
        if (cum >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* defined only for n ≥ 1 */

    /* locate diagonal and position inside it */
    idx_t d = find_diagonal(n);
    unsigned __int128 cum_before = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t offset = n - (idx_t)cum_before;   /* 1‑based offset in this diagonal */
    idx_t k = offset;                       /* column index                */
    idx_t i = d - offset;                   /* row index                   */

    /* case k = j² */
    idx_t j = floor_sqrt_u64(k);
    if ((unsigned __int128)j * j == k) {
        if (((i - 1) % j) == 0) return (seq_t)j;
        else                     return (seq_t)0;
    }

    /* case k = j² + j */
    unsigned __int128 disc = (unsigned __int128)1 + (unsigned __int128)4 * k;
    /* for n up to 10 000 the value of disc fits in 64 bits safely */
    idx_t s = floor_sqrt_u64((idx_t)disc);
    if ((unsigned __int128)s * s == disc && ((s - 1) % 2) == 0) {
        idx_t jj = (s - 1) / 2;
        if (((i - 1) % jj) == 0) {
            idx_t val = ((i - 1) / jj) + jj + 1;
            return (seq_t)val;
        } else
            return (seq_t)0;
    }

    return (seq_t)0;   /* all other columns contain only zeros */
}
