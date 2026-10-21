/*
 * A082601  Tribonacci array: to get the next row, right-adjust the previous 3
 *          rows and add them, then append a final 0.
 * https://oeis.org/A082601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((√(8n+1)-1)/2) and k = n -
 *            r*(r+1)/2. T(r,k) = Σ_{j=0}^{⌊k/2⌋} binomial(k-j , j) *
 *            binomial(r-k , k-j)
 * Range:     u8 n <= 81, u16 n <= 259, u32 n <= 874, u64 n <= 3107,
 *            u128 n <= 8000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the supplied binomial(idx_t,idx_t) helper. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n
       (r*(r+1))/2 ≤ n < ((r+1)*(r+2))/2  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
    {
        ++r;
    }

    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* column within this row, 0 ≤ k ≤ r */

    seq_t sum = 0;
    idx_t max_j = k / 2;
    for (idx_t j = 0; j <= max_j; ++j)
    {
        seq_t term1 = binomial(k - j, j);
        seq_t term2 = binomial(r - k, k - j);
        sum += term1 * term2;
    }
    return sum;
}
