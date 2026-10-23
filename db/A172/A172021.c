/*
 * A172021  Start with the triangle A171661, reverse its rows, add missing
 *          powers of 2 at beginning of each row.
 * https://oeis.org/A172021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n-th term obtained by taking triangle A171661,
 *            reversing its rows, and then prefixing each row with the missing
 *            powers of two. Equivalent to the sequence that starts
 *            1,1,2,2,1,2,4,6,6,1,2,4,8,14,20,20,1,2,4,8,16,30,50,70 …
 * Range:     u8 n <= 24, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root, floor(sqrt(n)) for 64‑bit unsigned integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t nxt = r | bit;
        if (n / nxt >= nxt) r = nxt;
    }
    return r;
}

/* Compute binomial coefficient C(n,k) using seq_t arithmetic */
static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, so n≥1 in practice */

    /* Find the row r (1‑based) containing the n‑th term.
       Cumulative number of terms up to row r-1 = (r-1)².
       r = floor(sqrt(n-1)) + 1 */
    idx_t r = isqrt_u64(n - 1) + 1;

    /* Index of the element within the row (0‑based) */
    idx_t prev = (r - 1) * (r - 1);
    idx_t k = n - prev - 1;

    /* For k < r-1 the values are powers of two */
    if (k < r - 1) {
        return ((seq_t)1) << k;
    }

    /* For k ≥ r-1 compute
         sum_{a = k-(r-1)}^{r-1} C(k,a)   */
    idx_t lower = k - (r - 1);
    idx_t upper = r - 1;
    seq_t sum = 0;
    for (idx_t a = lower; a <= upper; ++a) {
        sum += binom(k, a);
    }
    return sum;
}
