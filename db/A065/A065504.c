/*
 * A065504  a(n+1) = a(n) + n + the number of a(k)'s <= n, 1 <= k <= n and
 *          a(1) = 1.
 * https://oeis.org/A065504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n+1) = a(n) + n + #{k : 1 ≤ k ≤ n, a(k) ≤ n} (n ≥ 1)
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* array of terms, 1‑based; seq[0] unused */
    static idx_t max_n = 0;       /* largest n for which terms are filled */

    /* Allocate or grow the array as needed */
    if (max_n < n) {
        if (max_n == 0) {
            seq = (seq_t *)malloc((n + 1) * sizeof(seq_t));
            if (!seq) return (seq_t)0;        /* allocation failure */
            seq[1] = (seq_t)1;                /* a(1) = 1 */
            max_n = 1;
        }
        if (max_n < n) {
            seq = (seq_t *)realloc(seq, (n + 1) * sizeof(seq_t));
            if (!seq) return (seq_t)0;        /* allocation failure */
        }

        /* Compute terms from (max_n+1) up to n */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            /* m = i‑1;   count how many previous terms are ≤ m */
            seq_t m = (seq_t)(i - 1);          /* convert index to seq_t */
            idx_t lo = 1, hi = i - 1, cnt = 0;

            /* binary search for largest index with seq[idx] ≤ m */
            while (lo <= hi) {
                idx_t mid = lo + ((hi - lo) >> 1);
                if (seq[mid] <= m) {
                    cnt = mid;
                    lo = mid + 1;
                } else {
                    hi = mid - 1;
                }
            }

            /* recurrence: a(i) = a(i‑1) + (i‑1) + cnt */
            seq[i] = seq[i - 1] + m + (seq_t)cnt;
        }
        max_n = n;
    }
    return seq[n];
}
