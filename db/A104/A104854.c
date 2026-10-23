/*
 * A104854  Number of n-digit numbers using digits 0 to n-1 each exactly once
 *          and containing no 3-digit sequence in increasing or decreasing
 *          order.
 * https://oeis.org/A104854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A000111(n) – A000111(n‑1) where A000111(n) are the
 *            Euler up/down numbers. They satisfy: A000111(0)=1, A000111(1)=1,
 *            A000111(m) = (1/2) * Σ_{k=0}^{m‑1} C(m‑1,k) * A000111(k) *
 *            A000111(m‑1‑k) (m≥2)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"   /* binom(idx_t N, idx_t K) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* offset is 1, so value for n=0 is unused */
        return 0;

    static seq_t *e = NULL;   /* Euler up/down numbers */
    static idx_t cap = 0;     /* allocated size of e */

    /* ensure allocated memory can hold indices 0…n */
    if (cap < n + 1) {
        free(e);
        e = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        cap = n + 1;
    }

    /* initialise seeds */
    e[0] = (seq_t)1;
    e[1] = (seq_t)1;

    /* compute Euler numbers up to e[n] */
    for (idx_t m = 2; m <= n; ++m) {
        idx_t h   = m - 1;
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= h; ++k) {
            seq_t term = binom(h, k) * e[k] * e[h - k];
            sum += term;
        }
        e[m] = sum / (seq_t)2;   /* division is exact */
    }

    /* a(n) = 2*e[n] – e[n-1] */
    seq_t result = (seq_t)2 * e[n] - e[n - 1];
    return result;
}
