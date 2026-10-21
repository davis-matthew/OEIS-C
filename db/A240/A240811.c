/*
 * A240811  a(n) = length (or lifetime) of the meta-Fibonacci sequence f(1) =
 *          ... = f(n) = 1; f(k)=f(k-f(k-2))+f(k-f(k-n)) if that sequence is
 *          only defined for finitely many terms, or 0 if that sequence is
 *          infinite.
 * https://oeis.org/A240811
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the meta‑Fibonacci sequence * f(1) = … = f(n) =
 *            1 * f(k)=f(k‑f(k‑2)) + f(k‑f(k‑n)) for k>n * a(n) is the largest
 *            k such that f(1)…f(k) are defined. * If the sequence is
 *            infinite, a(n)=0. (Only n=4 is conjecturally infinite.)
 * Range:     u8 n <= 7, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

static const idx_t MAX_LIFETIME = 1000000;   /* upper bound to treat the sequence as infinite */

seq_t a(idx_t n)
{
    /* The sequence is only defined for n ≥ 2.  For smaller n return 0. */
    if (n < 2) return 0;

    /* Allocate storage: we compute indices up to n+MAX_LIFETIME.
     * f[0] is unused; f[1] … f[n] are initialised to 1. */
    idx_t max_index = n + MAX_LIFETIME;
    size_t fsize = (size_t)max_index + 2;   /* extras to avoid overflow in indexing */
    seq_t *f = (seq_t *)malloc(fsize * sizeof(seq_t));
    if (!f) return 0;                      /* out of memory – treat as infinite */

    for (idx_t i = 1; i <= n; ++i)
        f[i] = (seq_t)1;

    idx_t k = n + 1;               /* next index to compute */
    for (; k <= max_index; ++k)
    {
        /* Compute the two required predecessors.
           If f[k-2] >= k, then k‑f(k‑2) ≤ 0 → stop. */
        if ((idx_t)f[k-2] >= k) break;
        idx_t i1 = k - (idx_t)f[k-2];

        if ((idx_t)f[k-n] >= k) break;
        idx_t i2 = k - (idx_t)f[k-n];

        f[k] = f[i1] + f[i2];
    }

    /* Clean up memory. */
    free(f);

    /* If we reached the artificial limit, treat the sequence as infinite. */
    if (k > max_index) return 0;

    /* The sequence was defined up to index k‑1. */
    return (seq_t)(k - 1);
}
