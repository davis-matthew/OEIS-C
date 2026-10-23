/*
 * A168643  Triangle read by rows: T(n,k) = [x^k] p(x,n) where p(x,0) = 1,
 *          p(x,n) = (6 - n)*(1+x)^n - (5-n)*(1 + x^n) for 1 <= n <= 4, and p
 *          (x,n) = 4*(1+x)^n - Sum_{i=0..2} (Sum_{j=0..i} binomial(n, j)*(x^j
 *          + x^(n-j))) for n >= 5.
 * https://oeis.org/A168643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, less
 * Formula:   a(n) = the triangle read by rows T(0,0)=1; T(n,0)=T(n,n)=1; for
 *            1≤n≤4: T(n,k) = (6-n)*binomial(n,k) (1≤k≤n-1); for n≥5: T(n,k) =
 *            binomial(n,k)*((k+1)*[k<3] + 4*[2<k<n-2] + (n-k+1)*[k>n-3])
 *            (1≤k≤n-1)
 * Range:     u8 n <= 7, u16 n <= 158, u32 n <= 575, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Include helper functions
#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    seq_t sq  = floor_sqrt((seq_t)n * 8 + 1);          // sqrt(8n+1)
    idx_t r   = (idx_t)((sq - 1) / 2);                 // floor((sqrt(8n+1)-1)/2)
    idx_t start = (idx_t)(((seq_t)r * (r + 1)) / 2);   // offset of row r

    idx_t k = n - start;                              // column within the row

    /* Triangle T(r,k) */
    if (r == 0) return 1;            // only element
    if (k == 0 || k == r) return 1;  // boundaries

    if (r <= 4) {
        seq_t factor = (6 - (seq_t)r);
        return factor * binomial(r, k);
    }

    seq_t comb = binomial(r, k);
    seq_t mult;

    if (k < 3) {
        mult = k + 1;                // k=1 or 2
    } else if (2 < k && k < r - 2) {
        mult = 4;                    // 3 ≤ k ≤ r-3
    } else if (k > r - 3) {
        mult = r - k + 1;            // k = r-2, r-1
    } else {
        /* Should not occur for valid k */
        mult = 0;
    }

    return mult * comb;
}
