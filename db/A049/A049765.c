/*
 * A049765  Triangular array T, read by rows: T(n,k) = (k mod n) + (n mod k),
 *          for k = 1..n and n >= 1.
 * https://oeis.org/A049765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (k mod r) + (r mod k) where r is the row and k is the
 *            column for index n in the row‑major listing of the triangular
 *            array T with T(r,k) = (k mod r) + (r mod k). The mapping is: r =
 *            smallest integer with r(r+1)/2 >= n prev = (r-1)r/2 k = n - prev
 *            (1 <= k <= r)
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r: r(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n) {
        ++r;
    }

    /* Compute previous total terms up to row r-1 */
    idx_t prev = (r - 1) * r / 2;

    /* Column index k within row r (1‑based) */
    idx_t k = n - prev;

    /* Compute the value T(r,k) = (k mod r) + (r mod k) */
    seq_t k_mod_r = (seq_t)(k % r);
    seq_t r_mod_k = (seq_t)(r % k);

    return k_mod_r + r_mod_k;
}
