/*
 * A117268  Triangle, binomial transform of the tribonacci sequence.
 * https://oeis.org/A117268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   Let T(1)=1, T(2)=1, T(3)=2, and T(n)=T(n-1)+T(n-2)+T(n-3) for n>3.
   The triangle A117268 satisfies:
          a(n,1) = T(n)
          a(n,k) = Σ_{i=1}^{k-1} a(n-1,i)   for k > 1
   The OEIS sequence a(N) is obtained by reading the triangle in
   row‑major order (row 1, then row 2, …).  */
/*----------------------------------------*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the row r and column c (1‑based) containing index n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t start = ((r - 1) * r) / 2 + 1;
    idx_t c = n - start + 1;          /* column inside the found row */

    /* Temporary arrays for the previous and current row. */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((r + 1) * sizeof(seq_t));

    /* Tribonacci values up to r. */
    seq_t *trib = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    trib[1] = 1;
    if (r >= 2) trib[2] = 1;
    if (r >= 3) trib[3] = 2;
    for (idx_t i = 4; i <= r; ++i)
        trib[i] = trib[i - 1] + trib[i - 2] + trib[i - 3];

    /* Row 1. */
    prev[1] = trib[1];

    /* Build rows 2 .. r. */
    for (idx_t i = 2; i <= r; ++i) {
        cur[1] = trib[i];
        seq_t prefix = 0;
        for (idx_t k = 2; k <= i; ++k) {
            prefix += prev[k - 1];
            cur[k] = prefix;
        }
        /* Copy current row into previous for next iteration. */
        for (idx_t k = 1; k <= i; ++k)
            prev[k] = cur[k];
    }

    seq_t result = prev[c];

    free(prev);
    free(cur);
    free(trib);

    return result;
}
