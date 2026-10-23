/*
 * A101818  Triangle read by rows: (1/n)*T(n,h), where T(n,h) is the array in
 *          A101817.
 * https://oeis.org/A101818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Stirling2(r,h)*(r-1)!/(r-h)! where r is the least integer
 *            with r(r+1)/2 ≥ n, h = n – r(r−1)/2 (the column index in that
 *            row)
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; undefined for n=0 */

    /* Find r such that r(r+1)/2 >= n. */
    idx_t r = 1;
    idx_t cum = r * (r + 1) / 2;
    while (cum < n) {
        ++r;
        cum = r * (r + 1) / 2;
    }

    /* Column index within row r. */
    idx_t prev = r * (r - 1) / 2;
    idx_t h = n - prev;  /* 1 ≤ h ≤ r */

    /* Static table of Stirling numbers of the second kind. */
    static seq_t *S = NULL;
    static idx_t S_max = 0;   /* largest n already computed */
    static idx_t stride = 0;  /* stride = S_max + 1 */

    if (r > S_max) {
        /* Allocate a new, larger table and recompute. */
        free(S);
        S_max = r;
        stride = S_max + 1;
        S = (seq_t *)malloc((size_t)stride * stride * sizeof(seq_t));
        if (!S) return 0;  /* allocation failure */

        /* Initialise all entries to 0. */
        for (idx_t i = 0; i < stride * stride; ++i) S[i] = 0;

        /* Base case: S(0,0) = 1. */
        S[0 * stride + 0] = 1;

        /* Populate the table using the recurrence:
           S(n,k) = k * S(n-1,k) + S(n-1,k-1). */
        for (idx_t i = 1; i <= S_max; ++i) {
            for (idx_t j = 1; j <= i; ++j) {
                S[i * stride + j] =
                    (seq_t)j * S[(i - 1) * stride + j] + S[(i - 1) * stride + j - 1];
            }
        }
    }

    /* Compute T(r,h) = S(r,h) * (r-1)!/(r-h)!. */
    seq_t val = S[r * stride + h];

    /* Multiply by (r-1)!/(r-h)! which equals product of
       i = (r-h+1) .. (r-1).  The loop works even when h == r. */
    for (idx_t i = r - h + 1; i <= r - 1; ++i) {
        val *= i;
    }

    return val;
}
