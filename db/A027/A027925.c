/*
 * A027925  a(n) = least k such that E{1,2,...,k} >= 1^3 + 2^3 + ... + n^3,
 *          where E = 2nd elementary symmetric function.
 * https://oeis.org/A027925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _E128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A027925:  a(n) = max{ k : A000914(k) ≤ (1^3+2^3+…+n^3) }
   where A000914(k) = Σ_{1≤i<j≤k} i·j = ((k(k+1)/2)^2 – k(k+1)(2k+1)/6)/2. */

#include <stdint.h>
#include "../utils/_E128.h"

seq_t a(idx_t n) {
    if (n < 1)               /* safety for out‑of‑bounds index */
        return 0;

    /* target = ( Σ_{i=1}^n i )^2 = (n(n+1)/2)^2 */
    unsigned __int128 nn          = (unsigned __int128)n;
    unsigned __int128 sum_n       = nn * (nn + 1) / 2;
    unsigned __int128 target      = sum_n * sum_n;

    /* Find an upper bound high such that E(high) > target.  */
    uint64_t high = 1;
    while (_E128(high) <= target) {
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the largest k with E(k) ≤ target.  */
    uint64_t low = 1;
    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (_E128(mid) <= target)
            low = mid + 1;
        else
            high = mid;
    }

    return (seq_t)(low - 1);
}
