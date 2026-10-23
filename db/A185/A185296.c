/*
 * A185296  Triangle of connection constants between the falling factorials
 *          (x)_(n) and (2*x)_(n).
 * https://oeis.org/A185296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = flatten of the triangle of connection constants T(n,k), *
 *            defined by T(0,0)=1 and for n>0, * T(n,k) =
 *            (2*k-n+1)*T(n-1,k)+2*T(n-1,k-1) when k>=ceil(n/2), * 0
 *            otherwise. * The sequence a(n) is the T-values read row‑wise:
 *            a(0)=T(0,0), * a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), etc
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for arbitrary‑precision seq_t or built‑in integer types. */
seq_t a(idx_t n)
{
    /* Find the row r with r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    /* Column index k within that row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Allocate two rows of the triangle. */
    size_t sz = (size_t)(r + 1);           /* maximum needed width */
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;                  /* fallback on allocation failure */
    }

    /* Base row 0. */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r. */
    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t val;
            /* For k < ceil(n/2) the value is 0. */
            if (j < (i + 1) / 2) {
                val = (seq_t)0;
            } else {
                seq_t t1 = (j <= i - 1) ? prev[j] : (seq_t)0;   /* T(i‑1,j) */
                seq_t t2 = (j > 0)     ? prev[j - 1] : (seq_t)0; /* T(i‑1,j‑1) */
                seq_t mul = (seq_t)(2 * j - i + 1);             /* non‑negative */
                val = t1 * mul + t2 * (seq_t)2;
            }
            cur[j] = val;
        }
        /* Prepare for next iteration: swap the two buffers. */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Result is in prev[k] after the final row has been computed. */
    seq_t result = (k <= r) ? prev[k] : (seq_t)0;

    /* Free temporary storage. */
    free(prev);
    free(cur);

    return result;
}
