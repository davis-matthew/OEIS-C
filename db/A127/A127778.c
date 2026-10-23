/*
 * A127778  Triangle T(n,k) = A002411(k) read by rows.
 * https://oeis.org/A127778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(n, k) where T(r, k) = k^2 * (k + 1) / 2 and (r, k) are
 *            the row and column of n in the triangle read by rows with rows
 *            defined by the sequence A002411(k) = k^2*(k+1)/2. That is, let r
 *            be the unique integer with r*(r+1)/2 ≥ n, and let k = n –
 *            (r–1)*r/2. Then a(n) = k^2*(k+1)/2
 * Range:     u8 n <= 35, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed, but not used directly */
 
seq_t a(idx_t n)
{
    if (n == 0)           /* safety for out-of-range, not used by test data */
        return (seq_t)0;
 
    /* Find the row r: r*(r+1)/2 ≥ n, minimal such r */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;
 
    /* Previous total entries before row r */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;              /* column index within row r, 1-based */
 
    /* Compute k^2 * (k + 1) / 2 in seq_t */
    seq_t kk = (seq_t)k;
    seq_t res = kk * kk * (kk + 1) / 2;
    return res;
}
