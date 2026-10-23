/*
 * A184184  Triangle read by rows: T(n,k) is the number of permutations of [n]
 *          having k adjacent cycles (0 <= k <= n). An adjacent cycle is a
 *          cycle of the form (i, i+1, i+2, ...) (including 1-element cycles).
 * https://oeis.org/A184184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry T(r,k) of the triangle of adjacent-cycle *
 *            permutations, where r and k are determined by * r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2 and k = n – r(r+1)/2. * T(r,k) =
 *            (1/k!)*∑_{i=ceil((r-k-1)/2)}^{r-k} (-1)^{r-k-i} (k+i)! * *
 *            binomial(i+1, r-k-i)
 * Range:     u8 n <= 21, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

#define MAX_R 200   /* sufficient for n up to about 10000 (r≈141) */

seq_t a(idx_t n)
{
    /* --- locate (r,k) for the flattened index n ----------------------- */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;   /* 0‑based column */

    /* --- static state for factorials and pre‑computed triangle  ------ */
    static seq_t fact[MAX_R + 1];
    static seq_t T[MAX_R + 1][MAX_R + 1];
    static bool  init = false;
    static idx_t computed_up_to = 0;     /* rows 0…computed_up_to are ready */

    if (!init) {
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_R; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        T[0][0] = (seq_t)1;   /* row 0, column 0 is 1 */
        init = true;
    }

    /* --- compute rows up to r on demand ------------------------------ */
    while (computed_up_to < r) {
        ++computed_up_to;
        idx_t cur = computed_up_to;

        for (idx_t kk = 0; kk <= cur; ++kk) {
            /* compute the sum for T(cur,kk) */
            idx_t m      = cur - kk;                 /* n-k   */
            idx_t start  = m / 2;                    /* ceil((m-1)/2) */
            seq_t pos = 0, neg = 0;

            for (idx_t i = start; i <= m; ++i) {
                idx_t mid   = m - i;                 /* n-k-i   */
                seq_t term  = fact[kk + i] * binom(i + 1, mid);
                if (mid & 1) {
                    neg += term;
                } else {
                    pos += term;
                }
            }
            seq_t diff = pos - neg;                  /* guaranteed non‑negative */
            T[cur][kk] = diff / fact[kk];
        }
    }

    return T[r][k];
}
