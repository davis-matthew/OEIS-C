/*
 * A153182  Number of overpartitions of n with even M2-rank minus the number
 *          of overpartitions of n with odd M2-rank.
 * https://oeis.org/A153182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (1 + 8 Σ_{k>0} (-1)^k q^{k^2+2k} / (1+q^{2k})^2) / (1 + 2
 *            Σ_{k>0} (-1)^k q^{k^2}) (q = x)
 * Range:     i8 n <= 310, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 983 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == (idx_t)0) return (seq_t)1;

    size_t len = (size_t)n + 1;
    seq_t *D = mem_new(seq_t, len);  /* denominator 1+2*C(q) */
    seq_t *N = mem_new(seq_t, len);  /* numerator   1+8*B(q) */
    seq_t *S = mem_new(seq_t, len);  /* result a(q)   */

    if (!D || !N || !S) /* allocation failed */
    {
        if (D) mem_free(D);
        if (N) mem_free(N);
        if (S) mem_free(S);
        return (seq_t)0; /* fallback */
    }

    /* initial values */
    D[0] = (seq_t)1;
    N[0] = (seq_t)1;
    size_t sqCount = 0;
    size_t maxK = (size_t)sqrt((double)n);
    idx_t *squares = (idx_t*)malloc((maxK + 1) * sizeof(idx_t));
    if (!squares)
    {
        mem_free(D); mem_free(N); mem_free(S);
        return (seq_t)0;
    }

    for (idx_t k = 1; (idx_t)k * k <= n; ++k)
    {
        idx_t k2 = k * k;
        /* update denominator */
        int signC = (k & 1) ? -1 : 1;          /* (-1)^k */
        D[k2] = (seq_t)(2 * signC);
        squares[sqCount++] = k2;

        /* contributions to numerator from B(q) */
        idx_t base = k2 + 2 * k;               /* exponent for r = 1 */
        for (idx_t r = 1; base <= n; ++r, base += 2 * k)
        {
            int sign = (((k + r - 1) & 1) == 0) ? 1 : -1; /* (-1)^{k+r-1} */
            N[base] += (seq_t)(sign * (long long)r * 8LL);
        }
    }

    /* compute a(q) = N / D   (D[0] == 1) */
    S[0] = N[0];
    for (size_t i = 1; i <= n; ++i)
    {
        seq_t tmp = N[i];
        for (size_t s = 0; s < sqCount; ++s)
        {
            idx_t sq = squares[s];
            if (sq > i) break;
            tmp -= D[sq] * S[i - sq];
        }
        S[i] = tmp;
    }

    seq_t result = S[n];

    mem_free(D);
    mem_free(N);
    mem_free(S);
    free(squares);
    return result;
}
