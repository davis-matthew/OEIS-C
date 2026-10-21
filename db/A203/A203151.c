/*
 * A203151  (n-1)-st elementary symmetric function of
 *          {1,1,2,2,3,3,4,4,5,5,...,Floor[(n+1)/2]}.
 * https://oeis.org/A203151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=1}^{n} ∏_{j≠i} ⌊(j+1)/2⌋ (the (n‑1)‑st elementary
 *            symmetric function of the first n terms of the sequence
 *            1,1,2,2,3,3,…)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset 1, but safety */
    size_t N = (size_t)n;
    /* Allocate prefix and suffix product arrays */
    seq_t *pref = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *suff = (seq_t *)malloc((N + 2) * sizeof(seq_t));
    if (!pref || !suff) {
        /* Out of memory – return 0 as a safe fallback */
        if (pref) free(pref);
        if (suff) free(suff);
        return (seq_t)0;
    }

    /* pref[0] = 1; build prefix products */
    pref[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) {
        seq_t val = ((seq_t)(i + 1)) / 2;  /* ⌊(i+1)/2⌋ */
        pref[i] = pref[i - 1] * val;
    }

    /* suff[N+1] = 1; build suffix products */
    suff[N + 1] = (seq_t)1;
    for (size_t i = N; i >= 1; --i) {
        seq_t val = ((seq_t)(i + 1)) / 2;
        suff[i] = suff[i + 1] * val;
        if (i == 1) break;   /* prevent unsigned underflow */
    }

    /* Compute the (n‑1)-st elementary symmetric sum:
       sum of all products of n‑1 elements = Σ_i pref[i‑1] * suff[i+1] */
    seq_t sum = (seq_t)0;
    for (size_t i = 1; i <= N; ++i) {
        sum += pref[i - 1] * suff[i + 1];
    }

    free(pref);
    free(suff);
    return sum;
}
