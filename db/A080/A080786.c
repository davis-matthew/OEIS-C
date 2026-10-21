/*
 * A080786  Triangle T(n,k) = number of k-smooth numbers <= n, read by rows.
 * https://oeis.org/A080786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = count { m : 1 ≤ m ≤ r , largest‑prime‑factor(m) ≤ k }
 * Range:     u8 n <= 7260, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   row_col_for_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A080786 – Triangle T(n,k) = number of k‑smooth numbers ≤ n (read by rows)
   a(n) is the n‑th entry of the triangle, 1‑based.                            */
/* where r and k are the row and column of n in the triangle.                 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_col_for_index.h"

static uint32_t *lpf = NULL;          /* largest prime factor for each number */
static idx_t lpfSize = 0;             /* highest number for which lpf is filled */

/* Compute largest prime factor array up to 'n' (inclusive). */
static void compute_lpf(idx_t n)
{
    if (n <= (idx_t)lpfSize)
        return;

    uint32_t *newp = (uint32_t *)malloc((n + 1) * sizeof(uint32_t));
    if (!newp)
        return;
    for (idx_t i = 0; i <= n; ++i)
        newp[i] = 0;
    newp[1] = 1;          /* 1 is 1‑smooth */

    for (idx_t i = 2; i <= n; ++i) {
        if (newp[i] == 0) {            /* i is prime */
            for (idx_t j = i; j <= n; j += i) {
                if (newp[j] < (uint32_t)i)
                    newp[j] = (uint32_t)i;
            }
        }
    }

    free(lpf);
    lpf = newp;
    lpfSize = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset starts at 1 */

    idx_t r, k;
    row_col_for_index(n, &r, &k);

    /* Ensure lpf data up to r. */
    compute_lpf(r);

    /* Count numbers ≤ r whose largest prime factor ≤ k. */
    seq_t cnt = 0;
    for (idx_t m = 1; m <= r; ++m) {
        if (lpf[m] <= (uint32_t)k)
            ++cnt;
    }
    return cnt;
}
