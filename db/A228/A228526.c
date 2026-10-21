/*
 * A228526  Triangle read by rows: T(n,k) = sum of all parts of size k in all
 *          compositions (ordered partitions) of n.
 * https://oeis.org/A228526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = k if r==k = k*(r-k+3)*2^(r-k-2) if
 *            r>k r and k are obtained by r = smallest integer with r*(r+1)/2
 *            ≥ n (row number) k = n - (r-1)*r/2 (column number)
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   _int_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_int_isqrt_u64.h"

extern seq_t a(idx_t n)
{
    /* determine row r */
    idx_t r = (_int_isqrt_u64(8 * n + 1) - 1) / 2;

    /* adjust r so that r*(r+1)/2 >= n */
    while (r * (r + 1) / 2 < n) r++;
    while (r > 0 && (r - 1) * r / 2 >= n) r--;

    /* previous cumulative sum */
    idx_t prev = ((r - 1) * r) / 2;
    idx_t k = n - prev;   /* column index (1‑based) */

    if (k == r) {
        /* n == k → exactly one part of size k */
        return (seq_t)k;
    }

    idx_t t = r - k; /* t = n-k, t >= 1 */

    seq_t mult;
    if (t == 1) {
        /* f = 2 for t == 1 */
        mult = 2;
    } else {
        /* f = (t+3) * 2^(t-2) */
        seq_t pow_val = ((seq_t)1 << (t - 2));
        mult = ((seq_t)(t + 3)) * pow_val;
    }
    return mult * (seq_t)k;
}
