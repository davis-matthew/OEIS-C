/*
 * A138775  Triangle read by rows: T(n,k)=binomial(n-2k,3k) (n>=0, 0<=k<=n/5).
 * https://oeis.org/A138775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = binomial(n-2k, 3k) read row‑major from the triangle
 *            T(n,k)=binomial(n-2k,3k), where k = n - Σ_{i=0}^{r-1}(1+⌊i/5⌋),
 *            and r is the unique integer with Σ_{i=0}^r (1+⌊i/5⌋) > n. This
 *            implements the OEIS A138775 sequence
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains the nth term (0‑based). */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (1) {
        idx_t len = 1 + r / 5;                 /* number of terms in row r */
        /* If adding this row would pass n, we found the correct row. */
        if (cumulative + len > n)
            break;
        cumulative += len;
        r++;
    }

    /* Position within the row. */
    idx_t k = n - cumulative;                  /* 0‑based column in row r */

    /* Compute the binomial coefficient binomial(r-2k, 3k) */
    idx_t m   = r - 2 * k;
    idx_t kk  = 3 * k;
    return binomial(m, kk);
}
