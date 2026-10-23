/*
 * A124932  Triangle read by rows: T(n,k) = k*(k+1)*binomial(n,k)/2 (1 <= k <=
 *          n).
 * https://oeis.org/A124932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = k*(k+1)*binomial(r,k)/2, and r is
 *            the smallest integer with r*(r+1)/2 ≥ n, k = n - (r-1)*r/2
 *            (row‑major enumeration)
 * Range:     u8 n <= 24, u16 n <= 97, u32 n <= 392, u64 n <= 1738,
 *            u128 n <= 5050, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Find r with r*(r+1)/2 ≥ n via binary search. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 cum = (unsigned __int128)mid * (mid + 1) / 2;
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Compute k = n - (r-1)*r/2. */
    unsigned __int128 prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = (idx_t)((unsigned __int128)n - prev);

    /* Compute the triangle entry T(r,k). */
    seq_t binom_val = binomial(r, k);
    seq_t factor = ((seq_t)k * (k + 1)) / 2;   /* k*(k+1) is always even */
    return factor * binom_val;
}
