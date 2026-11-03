/*
 * A064319  Triangle with a(n,1) = n and a(n,k) = a(n,k-1) * a(n-1,k-1).
 * https://oeis.org/A064319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the triangle where a(n,1)=n and
 *            a(n,k)=a(n,k-1)*a(n-1,k-1) enumerated row‑wise
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 20, u64 n <= 26, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 30 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard for 0 */
    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    /* Position k within the row (1‑based). */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t k = n - prev_sum;               /* 1 <= k <= r */

    if (r == 1) return (seq_t)1;          /* a(1)=1 */

    /* Allocate two rows of size up to r. */
    seq_t *prev = mem_new(seq_t, r);         /* the rows are swapped: both hold r */
    seq_t *curr = mem_new(seq_t, r);
    if (!prev || !curr) {                  /* out of memory */
        /* In a contest setting we could abort; here we simply return 0. */
        return (seq_t)0;
    }

    /* Initialize first row (row 1). */
    prev[0] = 1;

    /* Build rows 2 .. r. */
    for (idx_t i = 2; i <= r; ++i) {
        curr[0] = (seq_t)i;                /* a(i,1) = i */
        /* Compute a(i,j) for j = 2 .. i using the recurrence. */
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = curr[j - 1] * prev[j - 1];
        }
        /* Swap pointers for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Now prev holds row r. */
    seq_t result = prev[k - 1];

    /* Free the temporary storage. */
    mem_free(prev);
    mem_free(curr);

    return result;
}
