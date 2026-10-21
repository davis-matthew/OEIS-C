/*
 * A218272  Infinitesimal generator for transpose of the Pascal matrix A007318
 *          (as upper triangular matrices).
 * https://oeis.org/A218272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = k if n = k*(k+3)/2 - 1 for some integer k >= 0, otherwise
 *            a(n) = 0
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derivation: the non‑zero terms of A218272 occur only at the indices
   n = 1, 4, 8, 13, 19, …, which satisfy n = k*(k+3)/2 - 1 for k = 1,2,3…
   Hence a(n) is k at those indices and 0 elsewhere.          */

#include <stdint.h>
#include <stdbool.h>

/* integer square root for 64‑bit values using binary search;
   result is floor(sqrt(x))                                     */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* the only non‑zero term with k = 0 would be a(0) = 0, so we can
       handle it directly. */
    if (n == 0) return 0;

    /* Solve k*(k+3)/2 - 1 = n  ->  k^2 + 3k - 2n - 2 = 0
       The positive root is (-3 + sqrt(8n + 17)) / 2.          */
    idx_t discr = 8 * n + 17;
    idx_t s = isqrt_u64(discr);              /* floor sqrt(discr)      */
    if (s < 3) return 0;                    /* no valid k possible    */
    idx_t k = (s - 3) / 2;                   /* floor of the root      */

    if (k == 0) return 0;                   /* would give n = -1      */

    /* compute n_k = k*(k+3)/2 - 1, using 128‑bit to avoid overflow  */
    unsigned __int128 nk = (unsigned __int128)k * (k + 3);
    nk = nk / 2;
    nk -= 1;

    if ((idx_t)nk == n)
        return (seq_t)k;                    /* match – return k       */
    else
        return 0;                           /* otherwise zero         */
}
