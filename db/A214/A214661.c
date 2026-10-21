/*
 * A214661  Odd numbers obtained by transposing the left half of A176271 into
 *          rows of a triangle: T(n,k) = A176271(n - 1 + k, k), 1 <= k <= n.
 * https://oeis.org/A214661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (r + k)^2 - 3*r - k + 1, where r = ceil((sqrt(1 + 8*n) -
 *            1)/2) and k = n - r*(r-1)/2. This enumerates the triangle T(n,k)
 *            in row-major order
 * Range:     u8 n <= 44, u16 n <= 8384, u32 n <= 8713, u64 n <= 8713,
 *            u128 n <= 8713, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the off‑by‑one offset: n >= 1 */
    if (n == 0)
        return (seq_t)0;

    idx_t r = 0;
    idx_t t = 0;          /* t = sum_{i=1}^{r} i = r*(r+1)/2 */

    /* Find the row r such that t_{r-1} < n <= t_r */
    while (t < n) {
        ++r;
        t += r;
    }

    /* t_prev = t_{r-1} = t - r */
    idx_t t_prev = t - r;
    idx_t k = n - t_prev;   /* column position in the row */

    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;

    /* Compute T(r,k) = (r+k)^2 - 3*r - k + 1 */
    seq_t val = (rr + kk) * (rr + kk) - 3 * rr - kk + 1;
    return val;
}
