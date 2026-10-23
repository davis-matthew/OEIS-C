/*
 * A169367  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of reduced words of length n in the Coxeter *
 *            group with 18 generators S_i, where S_i^2 = I and (S_i S_j)^31 =
 *            I. * * The counting can be done by an automaton that keeps track
 *            of the * current length of the longest suffix that alternates
 *            between two * distinct letters. Let r be that...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of previously computed sequence values */
    static seq_t cache[10001];
    /* current and next arrays of g[n][r], indices 2..30 are used */
    static seq_t g_cur[31];
    static seq_t g_next[31];
    static bool initialized = false;
    static idx_t max_computed = 1;   /* cache[0..max_computed] already set */

    if (!initialized)
    {
        /* initial values a[0] and a[1] */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;

        /* initialise state for n = 2 */
        for (idx_t r = 2; r <= 30; ++r) g_cur[r] = 0;
        g_cur[2] = (seq_t)18 * (seq_t)(17);   /* 306 */
        cache[2] = g_cur[2];
        max_computed = 2;

        /* the rest of the cache is left untouched until needed */
        initialized = true;
    }

    if (n <= max_computed)
        return cache[n];

    /* constants: K = 18 generators, A = K-2 = 16 choices that reset */
    const seq_t A = (seq_t)(18 - 2);   /* 16 */

    for (idx_t len = max_computed + 1; len <= n; ++len)
    {
        /* sum of all current states */
        seq_t sum = 0;
        for (idx_t r = 2; r <= 30; ++r) sum += g_cur[r];

        /* new states for len+1 */
        g_next[2] = sum * A;                         /* reset edge */
        for (idx_t r = 2; r <= 29; ++r)
            g_next[r + 1] = g_cur[r];                /* extend edge */
        g_next[30] = 0;                              /* r=30 cannot extend */

        /* total number of words of this length */
        seq_t a_next = sum * A + sum - g_cur[30];    /* = sum*17 - g_cur[30] */
        cache[len] = a_next;

        /* move to next iteration */
        for (idx_t r = 2; r <= 30; ++r) g_cur[r] = g_next[r];
    }

    max_computed = n;
    return cache[n];
}
