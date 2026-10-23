/*
 * A181840  Maximum of { k>0 | k<n and k is strongly prime to n }, or zero if
 *          this set is empty.
 * https://oeis.org/A181840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{ k>0 | k<n, gcd(k,n)=1, and (n-1)%k!=0 }, 0 if no
 *            such k. (k is strongly prime to n.)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   _gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_gcd_u64.h"

/* Euclidean algorithm for uint64_t (idx_t). */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;    /* a(0) = 0 */
    if (n == 1) return (seq_t)1;    /* convention: 1 is strongly prime to 1 */

    uint64_t nm1 = n - 1;           /* n-1 fits in idx_t (uint64_t) */

    for (uint64_t k = n - 1; k > 0; --k) {
        /* Check gcd(k,n) == 1 */
        if (_gcd_u64(k, n) != 1) continue;
        /* Check k does not divide n-1 */
        if (nm1 % k == 0) continue;
        return (seq_t)k;
    }
    return (seq_t)0;                /* no such k */
}
