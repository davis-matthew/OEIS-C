/*
 * A052876  Expansion of e.g.f. (exp(x)-1)^2/(-2+exp(x))^2.
 * https://oeis.org/A052876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k-1) * k! * S(n,k) // Stirling numbers of
 *            the second kind
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* caches for previously computed values */
    static seq_t *A        = NULL;  /* a[0..A_size-1]          */
    static size_t A_size   = 0;
    static seq_t *fact     = NULL;  /* factorials 0..fact_size-1 */
    static size_t fact_size= 0;
    static seq_t *prevS    = NULL;  /* previous Stirling row */
    static size_t prevS_size = 0;   /* size of prevS           */
    static seq_t *curS     = NULL;  /* current Stirling row   */
    static size_t curS_size = 0;

    /* first initialisation */
    if (A == NULL) {
        A = (seq_t *)malloc(sizeof(seq_t));
        if (!A) abort();
        A[0] = 0;
        A_size = 1;

        fact = (seq_t *)malloc(sizeof(seq_t));
        if (!fact) abort();
        fact[0] = 1;
        fact_size = 1;

        prevS = (seq_t *)malloc(sizeof(seq_t));
        if (!prevS) abort();
        prevS[0] = 1;
        prevS_size = 1;

        curS = NULL;
    }

    /* grow caches up to n */
    while (A_size <= n) {
        idx_t m = (idx_t)A_size;   /* next indices to compute */

        /* factorial for m */
        if (fact_size <= (size_t)m) {
            fact = (seq_t *)realloc(fact, (m + 1) * sizeof(seq_t));
            if (!fact) abort();
            fact_size = (size_t)m + 1;
        }
        fact[(size_t)m] = fact[(size_t)m - 1] * (seq_t)m;

        /* allocate current Stirling row */
        if (curS_size <= (size_t)m) {
            curS = (seq_t *)realloc(curS, (m + 1) * sizeof(seq_t));
            if (!curS) abort();
            curS_size = (size_t)m + 1;
        }

        curS[0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t part1 = (k < prevS_size) ? prevS[(size_t)k] : 0;
            seq_t part2 = (k - 1 < prevS_size) ? prevS[(size_t)(k - 1)] : 0;
            curS[(size_t)k] = part1 * (seq_t)k + part2;
        }

        /* compute a(m) from the recurrence */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = curS[(size_t)k] * fact[(size_t)k];
            term = term * (seq_t)(k - 1);
            sum += term;
        }

        /* store a(m) */
        A = (seq_t *)realloc(A, (A_size + 1) * sizeof(seq_t));
        if (!A) abort();
        A[(size_t)A_size] = sum;
        ++A_size;

        /* rotate Stirling buffers for next iteration */
        seq_t *tmp = prevS;
        prevS = curS;
        curS  = tmp;
        size_t old_prevS_size = prevS_size;
        prevS_size = curS_size;
        curS_size = old_prevS_size;
    }

    return A[(size_t)n];
}
