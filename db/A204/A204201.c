/*
 * A204201  Triangle based on (0,1/3,1) averaging array.
 * https://oeis.org/A204201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Triangle based on (0,1/3,1) averaging array.  */
/*  a(n) is the (n‑th) entry of the integer triangle S defined by   */
/*    S(1,1)            = 1                                 */
/*    S(n,1)            = 1                                 */
/*    S(n,k)            = S(n-1,k-1) + S(n-1,k)   (2≤k≤n-1)  */
/*    S(n,n)            = S(n-1,n-1) + 3·2^(n-2)              */
/*  The entries are enumerated row‑wise; a(1) = S(1,1), a(2)=S(2,1), a(3)=S(2,2), …  */
/*  Offset: 1  (a(1) is the first term)                          */

#include <stdlib.h>

/*--------------------------------------------------------------*/
static seq_t pow2_u64(idx_t e)
{
    /* Computes 2^e using only seq_t arithmetic (no overflow check).  */
    seq_t val = 1;
    while (e > 0)
    {
        val = val * (seq_t)2;
        e--;
    }
    return val;
}

/*--------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r such that 1 + 2 + … + r ≥ n  (row size r). */
    idx_t r = 1;
    idx_t cumulative = 1;          /* sum up to current row r */
    while (cumulative < n)
    {
        ++r;
        cumulative += r;
    }

    /* Position k of n within its row (1‑based). */
    idx_t prev_cumulative = cumulative - r;
    idx_t k = n - prev_cumulative;   /* 1 ≤ k ≤ r */

    /* Base case: first row contains only a(1). */
    if (r == 1)
    {
        return (seq_t)1;
    }

    /* Allocate two row buffers of length r+1 (index 1..r). */
    seq_t *prev = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !cur)  /* allocation failed: return 0 as safe fallback */
    {
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Row 1 */
    prev[1] = (seq_t)1;

    seq_t pow2 = (seq_t)1;   /* 2^(row-2) for the current row; starts with row=2 */

    /* Build rows 2 .. r */
    for (idx_t row = 2; row <= r; ++row)
    {
        /* First element is always 1. */
        cur[1] = (seq_t)1;

        /* Interior elements (if any). */
        for (idx_t j = 2; j < row; ++j)
        {
            cur[j] = prev[j - 1] + prev[j];
        }

        /* Last element of the row. */
        cur[row] = prev[row - 1] + (seq_t)3 * pow2;

        /* Prepare for next iteration: swap the buffers */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;

        /* Update pow2 for the next row: multiply by 2. */
        pow2 = pow2 * (seq_t)2;
    }

    /* Extract the required element.  */
    seq_t result = prev[k];

    /* Clean up memory. */
    free(prev);
    free(cur);

    return result;
}
