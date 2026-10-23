/*
 * A194799  Triangle read by rows: T(n,k) = number of partitions of n that are
 *          formed by k sections, k >= 1.
 * https://oeis.org/A194799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the smallest integer with r(r+1)/2 ≥ n,
 *            k = n - r(r-1)/2 (1‑based position in row r), and T(r,k) =
 *            A187219(r-k+1) = p(r-k+1) - p(r-k) with p(0)=1 (partition
 *            numbers). For r-k+1 = 1, return 1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)
        ++r;
    ++r;                            /* now r(r+1)/2 ≥ n */

    /* Position k within the row (1‑based). */
    idx_t prev = r * (r - 1) / 2;   /* sum of lengths of previous rows */
    idx_t k = n - prev;             /* 1 ≤ k ≤ r */

    /* Index in A187219 (i.e., in the partition difference array). */
    idx_t m = r - k + 1;            /* 1 ≤ m ≤ r */

    /* Ensure we have partition numbers p[0..r] (p[0] = 1). */
    static seq_t *p = NULL;
    static size_t cap = 0;          /* allocated size (number of entries) */

    if (cap <= (size_t)r) {
        size_t size = (size_t)r + 1;        /* indices 0..r */
        seq_t *newp = (seq_t *)malloc(size * sizeof(seq_t));
        if (!newp) exit(EXIT_FAILURE);

        newp[0] = (seq_t)1;                /* p(0) = 1 */
        for (idx_t i = 1; i <= (idx_t)r; ++i)
            newp[i] = (seq_t)0;

        /* Classic integer partition DP (additive). */
        for (idx_t part = 1; part <= (idx_t)r; ++part) {
            for (idx_t sum = part; sum <= (idx_t)r; ++sum) {
                newp[sum] = newp[sum] + newp[sum - part];
            }
        }

        if (p)
            free(p);
        p = newp;
        cap = size;
    }

    if (m == 1)        /* A187219(1) = 1 */
        return (seq_t)1;

    /* A187219(m) = p(m) - p(m-1) for m ≥ 2. */
    return p[m] - p[m - 1];
}
