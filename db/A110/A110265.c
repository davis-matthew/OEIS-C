/*
 * A110265  Smallest number greater than 1 that can't be written as
 *          floor(n/i)+floor(n/j), 1<=i<=j<=n.
 * https://oeis.org/A110265
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest integer >1 not representable as
 *            floor(n/i)+floor(n/j) with 1 ≤ i ≤ j ≤ n
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;            /* not used for n<2 */

    /* collect the distinct values of floor(n/k) */
    idx_t *vals = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    size_t m = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t v = n / k;
        if (m == 0 || vals[m - 1] != v) {
            vals[m++] = v;
        }
    }

    /* reachable sums can only be up to 2*n (+1 for the missing value) */
    size_t reach_size = (size_t)(2 * n + 2);           /* indices 0 … 2n+1 */
    bool *reach = (bool *)calloc(reach_size, sizeof(bool));

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = i; j < m; ++j) {
            size_t s = (size_t)(vals[i] + vals[j]);   /* 0 ≤ s ≤ 2n */
            if (s < reach_size) reach[s] = true;
        }
    }

    seq_t ans = 0;
    for (seq_t x = 2; x < reach_size; ++x) {
        if (!reach[(size_t)x]) { ans = x; break; }
    }
    if (ans == 0) ans = 2 * n + 2;   /* should not happen */

    free(vals);
    free(reach);
    return ans;
}
