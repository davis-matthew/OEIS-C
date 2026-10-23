/*
 * A180856  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          nodes at distance k in the Moebius ladder M(n) (entries in row n
 *          are the coefficients of the corresponding Wiener polynomial). The
 *          Moebius ladder M(n) (n>=3) is the graph obtained from the cycle
 *          graph C(2n) by adding new edges join...
 * https://oeis.org/A180856
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   T_candidate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A180856 – triangle read by rows.
 * Let T(n,1)=3·n.  For 2 ≤ k ≤ floor((n+1)/2):
 *   if n is odd and k=(n+1)/2, then T(n,k)=2·n,
 *   otherwise T(n,k)=4·n.
 * The linear index n starts at 3 (a(3) is the first term).
 * The routine maps the index to its row and column using the
 * cumulative count S(r)=∑_{i=3}^{r} floor((i+1)/2) and returns T(n,k).
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/T_candidate.h"

static inline seq_t row_cumulative(idx_t r)
{
    if (r < 3) return 0;
    idx_t m = r >> 1;                     /* floor(r/2) */
    if ((r & 1) == 0) {                   /* r = 2m  -> S = m*(m+1) - 2 */
        return (seq_t)m * (seq_t)(m + 1) - (seq_t)2;
    } else {                               /* r = 2m+1 -> S = (m+1)^2 - 2 */
        seq_t mm = (seq_t)(m + 1);
        return mm * mm - (seq_t)2;
    }
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;                  /* undefined below the offset */

    seq_t pos = (seq_t)n - 2;              /* position in the linear sequence, 1‑based */
    idx_t low  = 3;
    idx_t high = (idx_t)(pos * 2 + 10);     /* safe upper bound */
    if (high < low) high = low;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cum = row_cumulative(mid);
        if (cum >= pos) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    idx_t r = low;                          /* row containing the element */
    seq_t prev = row_cumulative(r - 1);     /* number of elements before row r */
    idx_t offset = (idx_t)(pos - prev);     /* column index (1‑based) */
    return T_candidate(r, offset);
}
