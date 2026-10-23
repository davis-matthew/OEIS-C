/*
 * A169359  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A169359: Number of reduced words of length n in the Coxeter group on
 * 10 generators S_i with relations S_i^2 = I and (S_iS_j)^3 = I.
 *
 * Recurrence:
 *   a(0) = 1
 *   a(1) = 10
 *   a(n) = 9*a(n-1)                      for 2 ≤ n ≤ 30
 *   a(n) = 8*∑_{k=1}^{30} a(n-k) – 36*a(n-31)
 * for n ≥ 31
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static int      initialized = 0;
    static idx_t    max_n       = 0;
    static size_t   memo_cap    = 0;
    static seq_t   *memo        = NULL;

    /* ---------- initialisation ---------- */
    if (!initialized) {
        /* Allocate room for indices 0 … 30 first */
        memo_cap = 32;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        if (!memo)
            return 0;                      /* unlikely in tests                */

        /* a(0) and a(1) */
        memo[0] = 1;
        memo[1] = 10;

        /* a(2) … a(30) = 9 * previous term          */
        for (idx_t i = 2; i <= 30; ++i)
            memo[i] = memo[i-1] * 9;

        max_n = 30;
        initialized = 1;
    }

    if (n <= max_n)
        return memo[n];

    /* ---------- enlarge memoisation if required ---------- */
    if ((size_t)n >= memo_cap) {
        size_t new_cap = memo_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(memo, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0;                      /* unlikely in tests                */
        for (size_t i = memo_cap; i < new_cap; ++i)
            tmp[i] = 0;
        memo = tmp;
        memo_cap = new_cap;
    }

    /* ---------- compute missing terms using the recurrence ---------- */
    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum = 0;
        for (int j = 1; j <= 30; ++j)
            sum += memo[k - j];
        memo[k] = sum * 8 - memo[k - 31] * 36;
    }

    max_n = n;
    return memo[n];
}
