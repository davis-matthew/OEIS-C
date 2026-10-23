/*
 * A143944  Triangle read by rows: T(n,k) is the number of unordered pairs of
 *          vertices at distance k from each other in the grid P_n X P_n (1 <=
 *          k <= 2n-2), where P_n is the path graph on n vertices.
 * https://oeis.org/A143944
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A143944 – Triangle read by rows
   a(n) is the number of unordered pairs of vertices at distance k
   in the grid P_r × P_r, where r is the row containing n.
   The row r has 2r‑2 entries and starts at global index
     start(r) = r² – 3r + 4     (valid for r ≥ 2).
*/

#include <stdint.h>

/* start index of row r (r ≥ 2) */
static inline idx_t start_index(idx_t r)
{
    return r * r - 3 * r + 4;
}

/* find the row that contains global index n (n ≥ 2) */
static idx_t find_row(idx_t n)
{
    idx_t low = 2, high = 2;
    while (start_index(high) <= n)
        high <<= 1;                       /* exponential search for upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (start_index(mid) <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;                       /* low-1 has start_index ≤ n */
}

/* number of unordered vertex pairs at Manhattan distance d in r×r grid */
static seq_t count_pairs(idx_t r, idx_t d)
{
    seq_t sum = 0;
    /* |Δx| ranges from max(0, d-(r-1)) to min(d, r-1) */
    idx_t i_min = d > (r - 1) ? d - (r - 1) : 0;
    idx_t i_max = d < (r - 1) ? d : (r - 1);
    for (idx_t i = i_min; i <= i_max; ++i) {
        idx_t j = d - i;                 /* |Δy| */
        if (j > (r - 1))
            continue;
        if (i == 0 && j == 0)
            continue;                    /* same vertex, not a pair */

        seq_t term;
        if (i == 0) {                    /* vertical only */
            term = (seq_t)r * ((seq_t)r - j);
        } else if (j == 0) {             /* horizontal only */
            term = (seq_t)r * ((seq_t)r - i);
        } else {                         /* both non‑zero */
            term = (seq_t)2 * ((seq_t)(r - i)) * ((seq_t)(r - j));
        }
        sum += term;
    }
    return sum;
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* sequence defined for n ≥ 2 */

    idx_t r = find_row(n);
    idx_t offset_start = start_index(r);
    idx_t k = n - offset_start + 1;      /* k ∈ [1, 2r‑2] */
    return count_pairs(r, k);
}
