/*
 * A181837  T(n,k) = [k is strongly prime to n], the indicator function of
 *          strong coprimality, triangle read by rows.
 * https://oeis.org/A181837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* T(n,k) = [k is strongly prime to n]
   a(n) is the flattened triangle read by rows.
   For a given n we find row r and column k:
       r = ⌊(√(8·n+1)−1)/2⌋      (0‑based)
       k = n − r·(r+1)/2
   Then a(n) = 1 iff gcd(r,k)=1 and k∤(r−1), otherwise 0.
   (k=0 is always 0.) */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        r++;
    }

    /* offset of the first element in this row */
    idx_t off = r * (r + 1) / 2;

    /* column index k */
    idx_t k = n - off;

    if (k == 0) {
        return (seq_t)0;
    }

    /* check coprimality: gcd(r,k) == 1 */
    if (gcd((seq_t)r, (seq_t)k) != (seq_t)1) {
        return (seq_t)0;
    }

    /* check that k does not divide r-1 */
    if ((r - 1) % k == 0) {
        return (seq_t)0;
    }

    return (seq_t)1;
}
