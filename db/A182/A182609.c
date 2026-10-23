/*
 * A182609  Number of conjugacy classes in GL(n,19).
 * https://oeis.org/A182609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of ∏_{k≥1} (1–x^k)/(1–19x^k)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by dynamic programming on part size. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* idx_t and seq_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!prev || !cur) return (seq_t)0;          /* allocation failed */

    prev[0] = (seq_t)1;                          /* only the empty partition */
    const seq_t q      = (seq_t)19;
    const seq_t q_minus_one = (seq_t)18;          /* q-1 */

    for (idx_t k = 1; k <= n; ++k) {
        /* initialise current row to zero */
        memset(cur, 0, (N + 1) * sizeof(seq_t));

        /* process each residue class modulo k */
        for (idx_t r = 0; r < k; ++r) {
            seq_t S = (seq_t)0;                 /* weighted sum accumulator */
            for (idx_t t = 0; ; ++t) {
                idx_t idx = r + t * k;
                if (idx > N) break;

                seq_t h   = prev[idx];           /* f_{k-1}(idx) */
                seq_t val = h;                   /* m = 0 contribution */

                if (t > 0) {                    /* add m ≥ 1 part contributions */
                    val += q_minus_one * S;
                }

                cur[idx] = val;                  /* f_k(idx) */

                /* update S for next step (t+1) */
                S = h + q * S;
            }
        }

        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[N];
    free(prev);
    free(cur);
    return result;
}
