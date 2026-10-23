/*
 * A137596  Triangle read by rows: T(n, k) = Sum_{i=0..n} Stirling2(i, k).
 * https://oeis.org/A137596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where * r = max r with r(r+1)/2 <= n * k = n -
 *            r(r+1)/2 * T(r,k) = Σ_{i=k..r} Stirling2(i,k) * (Stirling
 *            numbers of the second kind are computed by * S(n,k) =
 *            k*S(n-1,k)+S(n-1,k-1), S(0,0)=1, S(n,0)=0 for n>0)
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* determine row r and column k from linear index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;
    idx_t k = n - r * (r + 1) / 2;   /* 0 <= k <= r */

    /* T(r,0) is always 1 */
    if (k == 0)
        return (seq_t)1;

    /* allocate two rows of length r+1, zero‑initialized */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!prev || !curr) {
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;    /* allocation failure (arbitrary) */
    }

    /* base row S(0,0)=1 */
    prev[0] = (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t i = 0; i <= r; ++i) {
        /* first element of each row */
        curr[0] = (seq_t)0;
        if (i == 0)
            curr[0] = (seq_t)1;  /* S(0,0) */
        else {
            for (idx_t j = 1; j <= i; ++j) {
                curr[j] = (seq_t)j * prev[j] + prev[j - 1];
            }
        }

        if (i >= k)
            sum += curr[k];

        if (i < r) {
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }
    }

    free(prev);
    free(curr);
    return sum;
}
