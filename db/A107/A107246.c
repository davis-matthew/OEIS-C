/*
 * A107246  Sum of squares of octanacci numbers (Fibonacci 8-step numbers).
 * https://oeis.org/A107246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{n} F_8(k)^2 where F_8(0..6)=0, F_8(7)=1, and
 *            F_8(m)=F_8(m-1)+…+F_8(m-8) for m≥8 a(0)=0, a(1)=0, …, a(6)=0,
 *            a(7)=1 …
 * Range:     u8 n <= 11, u16 n <= 15, u32 n <= 23, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool initialized = false;
    static seq_t *F = NULL;
    static seq_t *S = NULL;
    static size_t array_len = 0;      /* current allocated size, indices 0..array_len-1 */
    static idx_t max_computed = 0;    /* largest index already filled */

    if (!initialized)
    {
        /* allocate space for indices 0..7 */
        array_len = 8;                /* indices 0..7 inclusive */
        F = (seq_t *)calloc(array_len, sizeof(seq_t));
        S = (seq_t *)calloc(array_len, sizeof(seq_t));
        /* initial octanacci terms */
        if (array_len > 7)
        {
            F[7] = (seq_t)1;
            S[7] = (seq_t)1;          /* sum of squares up to index 7 */
        }
        max_computed = 7;
        initialized = true;
    }

    /* enlarge arrays if needed */
    if ((size_t)n + 1 > array_len)
    {
        size_t new_len = (size_t)n + 1;
        seq_t *tmpF = (seq_t *)realloc(F, new_len * sizeof(seq_t));
        seq_t *tmpS = (seq_t *)realloc(S, new_len * sizeof(seq_t));
        if (!tmpF || !tmpS)        /* allocation failure – abort */
        {
            /* on failure return 0; (unlikely in the test environment) */
            return (seq_t)0;
        }
        F = tmpF;
        S = tmpS;
        /* zero the newly allocated part */
        memset(F + array_len, 0, (new_len - array_len) * sizeof(seq_t));
        memset(S + array_len, 0, (new_len - array_len) * sizeof(seq_t));
        array_len = new_len;
    }

    /* compute forward from last computed index up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* compute F[i] if i>=8 by summing previous 8 terms */
        if (i >= 8)
        {
            seq_t sum = (seq_t)0;
            for (int j = 1; j <= 8; ++j)
                sum += F[i - j];
            F[i] = sum;
        }
        /* for i < 8, F[i] is already 0 (or 1 at i==7) */

        /* compute cumulative sum of squares */
        S[i] = S[i - 1] + (F[i] * F[i]);
    }

    max_computed = (idx_t) (n > max_computed ? n : max_computed);
    return S[n];
}
