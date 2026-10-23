/*
 * A166419  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*∑_{j=1..10} a(n‑j) – 276*a(n‑11) for n ≥ 12 Initial
 *            values: a(0) = 1 a(1) = 25 a(2) = 600 a(3) = 14 400 a(4) = 345
 *            600 a(5) = 8 294 400 a(6) = 199 065 600 a(7) = 4 777 574 400
 *            a(8) = 114 661 785 600 a(9) = 2 751 882 854 400 a(10) = 66 045
 *            188 505 600 a(11) = 1 585 084 524 134 100
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *seq = NULL;
    static idx_t capacity = 0;
    static idx_t last_computed = 0;

    /* first call initialises the cache */
    if (seq == NULL) {
        capacity = 16;                         /* small start size */
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (seq == NULL) return 0;            /* out‑of‑memory */

        /* base values 0 .. 11 */
        seq[0] = 1;
        seq[1] = 25;
        /* a(2) .. a(10) = previous * 24 */
        for (idx_t i = 2; i <= 10; ++i)
            seq[i] = seq[i - 1] * (seq_t)24;
        seq[11] = 1585084524134100ULL;         /* a(11) hard‑coded */
        last_computed = 11;
    }

    /* If the requested index is already known, return it immediately */
    if (n <= last_computed)
        return seq[n];

    /* Grow the cache if necessary */
    if (capacity <= n) {
        idx_t new_cap = capacity ?: 1;
        while (new_cap <= n)
            new_cap <<= 1;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (seq == NULL) return 0;            /* out‑of‑memory */
        capacity = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t s = 0;
        for (int j = 1; j <= 10; ++j)
            s += seq[i - j];
        seq[i] = 23 * s - (276 * seq[i - 11]);
    }

    last_computed = n;
    return seq[n];
}
