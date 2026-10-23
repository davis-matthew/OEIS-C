/*
 * A120911  Triangle read by rows where t(n,m) is the (floor(n/m))th integer
 *          from among those positive integers not occurring earlier in the
 *          sequence.
 * https://oeis.org/A120911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   * Triangle entries: T(r,k) = r(r-1)/2 + U(r,k) * with * U(r,1) =
 *            r * U(r,k) = k-1 if ⌊r/k⌋ = ⌊r/(k-1)⌋ * U(r,k) = ⌊r/k⌋ otherwise
 *            * The linear sequence a(n) is the triangle read by rows. * a(n)
 *            = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n * and k =
 *            n - (r-1)r/2 (1‑based column within the...
 * Range:     u8 n <= 253, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1 – not used for OEIS */

    /* Find the row r (1‑based) such that (r−1)r/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    while (((unsigned __int128)r * (r + 1) / 2) < n)
        ++r;

    /* Triangle offset of the beginning of row r. */
    idx_t prev_tri = r * (r - 1) / 2;          /* (r-1)r/2   */
    /* Column index k (1‑based). 1 ≤ k ≤ r. */
    idx_t k = n - prev_tri;

    /* Compute U(r,k) according to the rule. */
    seq_t U;
    if (k == 1) {
        U = r;
    } else {
        idx_t a = r / k;
        idx_t b = r / (k - 1);
        U = (a == b) ? (k - 1) : a;
    }

    /* Result = offset + U. */
    return (seq_t)prev_tri + U;
}
