/*
 * A027467  Triangle whose (n,k)-th entry is 15^(n-k)*binomial(n,k).
 * https://oeis.org/A027467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 15^(r-k) * binomial(r,k) where r is the row such that
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. This is the
 *            row‑major flattening of the triangle T(r,k) = 15^(r-k)*C(r,k)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 152, u128 n <= 560,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1);
        prod /= 2;
        if (prod <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;

    /* Compute 15^(r-k) and binomial(r,k) and multiply */
    seq_t pow15 = pow_seq((seq_t)15, (int)(r - k));
    seq_t bin = binomial(r, k);
    return pow15 * bin;
}
