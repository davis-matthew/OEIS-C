/*
 * A205548  Symmetric matrix by antidiagonals: C(max(i+1,j+1),min(i+1,j+1)),
 *          i>=1, j>=1.
 * https://oeis.org/A205548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C(max(i+1,j+1),min(i+1,j+1)) where (i,j) are obtained by
 *            numbering the infinite symmetric matrix by antidiagonals
 *            starting at (1,1)
 * Range:     u8 n <= 38, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   binomial, count_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/count_terms.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for offset 1, safe fallback */

    /* Binary search for minimal s such that n <= s*(s-1)/2 */
    uint64_t low = 2;
    uint64_t high = (n > 2 ? n : 2);

    while (low < high) {
        uint64_t mid = low + (high - low) / 2;
        if (count_terms(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }

    uint64_t s = low;                     /* sum i + j = s */
    unsigned __int128 tprev = count_terms(s - 1);  /* terms before this diagonal */
    uint64_t k = n - (uint64_t)tprev;     /* position on the diagonal (1‑based) */

    uint64_t i = k;              /* row index (1‑based) */
    uint64_t j = s - k;          /* column index (1‑based) */

    uint64_t u = i + 1;          /* convert to 2‑based index for binomial */
    uint64_t v = j + 1;

    uint64_t n_bin = u > v ? u : v;
    uint64_t k_bin = u > v ? v : u;

    return binomial(n_bin, k_bin);
}
