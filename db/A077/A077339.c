/*
 * A077339  Triangle in which n-th row contains the first n numbers beginning
 *          with n.
 * https://oeis.org/A077339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A077339
 *
 * The triangle is formed by rows:
 *   Row m contains the first m numbers beginning with m.
 *   After the first element m, the following elements are obtained
 *   by appending digits in increasing order:
 *        10*m, 10*m+1, ... ,10*m+9,
 *        100*m, 100*m+1, ... ,100*m+99,
 *        1000*m, ...
 *
 * a(n) is the n‑th element in the flattened triangle (offset = 1).
 * We compute it in O(√n) time by determining the row that
 * contains index n, then determining the element position within that row.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Find the row r such that
     *   1 + 2 + … + (r – 1) < n ≤ 1 + 2 + … + r
     * i.e. T(r–1) < n ≤ T(r), where T(k) = k*(k+1)/2.
     */
    if (n == 1) return (seq_t)1;          /* special‑case to avoid sqrt(0) */

    /* Compute r ≈ ceil( (sqrt(8n+1) – 1) / 2 ) */
    long double ld = sqrtl((long double)8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((ld - 1.0L) / 2.0L);   /* floor */
    /* Adjust if necessary */
    idx_t tri = (idx_t)(((__int128)r * (r + 1)) / 2);
    if (tri < n) r++;

    /* Position inside the row (0‑based) */
    idx_t start = (idx_t)(((__int128)(r - 1) * r) / 2) + 1; /* first index of row r */
    idx_t pos   = n - start;      /* 0 … r-1 */

    if (pos == 0)
        return (seq_t)r;          /* first element of the row */

    /* Remaining positions after the first element */
    idx_t rem = pos - 1;
    seq_t factor = 10;            /* 10^1 */

    while (1) {
        seq_t count = factor;               /* number of elements in this block */
        if ((seq_t)rem < count) {           /* answer lies in this block */
            return (seq_t)factor * r + (seq_t)rem;
        }
        rem -= (idx_t)count;                /* step to next block */
        factor *= 10;                       /* next power of 10 */
    }
}
