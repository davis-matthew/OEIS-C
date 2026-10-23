/*
 * A170667  Number of reduced words of length n in Coxeter group on 34
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1. For 1 ≤ n ≤ 48: a(n) = 2 + 32·∑_{k=1}^{n} a(n‑k). For
 *            n ≥ 49: a(n) = 32·∑_{k=1}^{48} a(n‑k) – 528·a(n‑49)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

#ifdef __cplusplus
    std::vector<seq_t> v(n + 1);
    v[0] = (seq_t)1;
    seq_t currSum = v[0];                     /* sum of last up to 48 terms so far */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai;
        if (i <= (idx_t)48)
            ai = (seq_t)2 + (seq_t)32 * currSum;
        else
            ai = (seq_t)32 * currSum - (seq_t)528 * v[i - 49];
        v[i] = ai;
        currSum += ai;
        if (i >= (idx_t)48)
            currSum -= v[i - 48];
    }
    return v[n];
#else   /* C compilation */
    seq_t *v = malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!v)
        abort();                              /* out of memory */
    v[0] = (seq_t)1;
    seq_t currSum = v[0];
    for (idx_t i = 1; i <= n; ++i) {
        seq_t ai;
        if (i <= (idx_t)48)
            ai = (seq_t)2 + (seq_t)32 * currSum;
        else
            ai = (seq_t)32 * currSum - (seq_t)528 * v[i - 49];
        v[i] = ai;
        currSum += ai;
        if (i >= (idx_t)48)
            currSum -= v[i - 48];
    }
    seq_t result = v[n];
    free(v);
    return result;
#endif
}
