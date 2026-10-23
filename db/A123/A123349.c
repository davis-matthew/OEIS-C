/*
 * A123349  Square array of Kekulé numbers for the mirror-symmetrical chevrons
 *          Ch(m,n), read by antidiagonals (m,n >= 0).
 * https://oeis.org/A123349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ∑_{i=0}^{s-m} binomial(m+i-1,i)^2 , where s =
 *            ⌊(√(8n+1)-1)/2⌋ (the diagonal index) and m = n - s*(s+1)/2 (row
 *            index on that diagonal)
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 218, u64 n <= 682,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find s such that triangular(s) ≤ n < triangular(s+1)   */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    idx_t s = high;                               /* diagonal index   */
    unsigned __int128 tri_s = (unsigned __int128)s * (s + 1) / 2;
    idx_t m = n - (idx_t)tri_s;                   /* row index        */
    idx_t kmax = s - m;                           /* number of terms to sum */

    seq_t sum = 1;  /* i = 0 term: binomial = 1, 1^2 = 1 */
    if (kmax == 0)
        return sum;

    seq_t binom = 1;
    for (idx_t i = 1; i <= kmax; ++i) {
        /* binom = binom * (m + i - 1) / i   (exact division) */
        binom = (binom * (seq_t)(m + i - 1)) / (seq_t)i;
        sum += binom * binom;
    }
    return sum;
}
