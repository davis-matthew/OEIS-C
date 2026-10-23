/*
 * A027360  Triangular array T read by rows: T(n,k) = number of partitions of
 *          n into distinct odd parts, the greatest being >= k.
 * https://oeis.org/A027360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = number of partitions of n into distinct odd parts with
 *            greatest part ≥ k. a(n) is the triangular array T read by rows
 *            (row r has r entries)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes a(n) for n ≥ 1.  The function may be called in any order. */
seq_t a(idx_t n)
{
    /* 1. Find the row r such that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    idx_t cum = 1;                   /* cumulative number of terms up to row r */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev_tri = cum - r;          /* terms before this row */
    idx_t k = n - prev_tri;             /* column within the row (1‑based) */

    /* 2. Compute total partitions of r into distinct odd parts (P(r)). */
    seq_t *dp_total = (seq_t *)calloc((size_t)r + 1, sizeof(seq_t));
    if (!dp_total) abort();                /* out of memory */
    dp_total[0] = 1;
    for (idx_t odd = 1; odd <= r; odd += 2) {
        for (idx_t i = r; i >= odd; --i) {
            dp_total[i] += dp_total[i - odd];
        }
    }
    seq_t total = dp_total[r];
    free(dp_total);

    /* 3. If k==1, all partitions satisfy the condition. */
    if (k <= 1) return total;

    /* 4. Compute partitions with all parts < k, i.e. using only odd parts
       less than k.  The largest allowed odd part is the largest odd < k. */
    idx_t max_odd = k - 1;                  /* largest odd < k may start here */
    if (max_odd % 2 == 0) --max_odd;        /* make it odd */
    if (max_odd == 0 || r == 0) return total;   /* no odd parts available */

    if (max_odd > r) max_odd = r;           /* restrict to row size */

    seq_t *dp_less = (seq_t *)calloc((size_t)r + 1, sizeof(seq_t));
    if (!dp_less) abort();
    dp_less[0] = 1;
    for (idx_t odd = 1; odd <= max_odd; odd += 2) {
        for (idx_t i = r; i >= odd; --i) {
            dp_less[i] += dp_less[i - odd];
        }
    }
    seq_t less = dp_less[r];
    free(dp_less);

    /* 5. The desired count is P(r) - partitions with all parts < k. */
    seq_t result = total > less ? total - less : 0;
    return result;
}
