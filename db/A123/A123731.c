/*
 * A123731  The Kruskal-Macaulay function M_5(n).
 * https://oeis.org/A123731
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   M_5(n) = Σ_{r=1..5} C(k_r-1, r-1) where k_r is the largest
 *            integer with C(k_r, r) ≤ remaining after subtracting
 *            higher‑order binomials. The function decomposes n greedily into
 *            binomial coefficients
 * Range:     u8 n <= 61, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t remain = (seq_t)n;          /* work with sequence type to avoid casts   */
    seq_t result = 0;

    for (int r = 5; r >= 1; --r) {
        if (remain == 0) break;       /* nothing left to decompose */
        idx_t k = r;                  /* start from the minimal admissible k */

        /* Find the largest k such that C(k,r) ≤ remain */
        while (1) {
            seq_t bin = binomial(k, (idx_t)r);
            if (bin > remain) break;
            ++k;
        }
        --k;                          /* step back to last valid k   */

        seq_t bin = binomial(k, (idx_t)r);
        remain -= bin;                /* subtract the chosen binomial */
        result += binomial(k - 1, (idx_t)(r - 1));   /* add the M_5 part */
    }

    return result;
}
