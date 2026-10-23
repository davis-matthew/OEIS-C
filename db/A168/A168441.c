/*
 * A168441  Expansion of 1/(1-x/(1-2x/(1-4x/(1-6x/(1-8x/(1-.... (continued
 *          fraction).
 * https://oeis.org/A168441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = upper‑left entry of M^n, where M is the infinite * matrix
 *            with M[i][j] = (i==0?1:2*i) for j <= i+1 and 0 otherwise. * The
 *            implementation builds the row vector v_k = M^k·e_0 iteratively.
 *            * For each step the new row is obtained from weighted prefix
 *            sums, * using only +, -, *, / and integer i...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* seq_t and idx_t are defined by the build; do not redefine them. */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate workspace of length n+1 (max needed). */
    idx_t max_len = n + 1;
    seq_t *v = (seq_t*)malloc(max_len * sizeof(seq_t));
    seq_t *new_v = (seq_t*)malloc(max_len * sizeof(seq_t));
    seq_t *prefix = (seq_t*)malloc(max_len * sizeof(seq_t));
    if (!v || !new_v || !prefix)
        return (seq_t)0;  /* fallback; unlikely in tests */

    /* initial row: M^0 = I, so v[0] = 1 */
    v[0] = (seq_t)1;
    idx_t cur_len = 1;  /* current length of v (k+1 for step k) */

    for (idx_t step = 1; step <= n; ++step)
    {
        /* Compute weighted prefix sums for current row. */
        for (idx_t j = 0; j < cur_len; ++j) {
            seq_t cj = (j == 0) ? (seq_t)1 : (seq_t)2 * (seq_t)j;
            seq_t prod = v[j] * cj;
            if (j == 0)
                prefix[0] = prod;
            else
                prefix[j] = prefix[j-1] + prod;
        }

        seq_t weighted_total = prefix[cur_len-1];  /* a(step) */

        /* Build next row. */
        new_v[0] = weighted_total;
        for (idx_t i = 1; i <= cur_len; ++i) {
            if (i >= 2)
                new_v[i] = weighted_total - prefix[i-2];
            else  /* i == 1 */
                new_v[i] = weighted_total;
        }

        /* Swap buffers for next iteration. */
        seq_t *tmp = v;
        v = new_v;
        new_v = tmp;
        cur_len += 1;
    }

    seq_t result = v[0];  /* a(n) */

    free(v);
    free(new_v);
    free(prefix);

    return result;
}
