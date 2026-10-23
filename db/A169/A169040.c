/*
 * A169040  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1;
   for 1 ≤ n ≤ 23:   a(n)=25·(a(n‑1)+…+a(0))+2;
   for n ≥ 24:     a(n)=25·(a(n‑1)+…+a(n‑23))−325·a(n‑24)+(n==24?1:0) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated size */
    static size_t len   = 0;   /* number of terms currently stored */

    /* Allocate or grow cache to hold terms up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap < ((size_t)n + 1))
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = len; i <= (size_t)n; ++i) {
        if (i == 0) {               /* base case */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = (seq_t)0;
        size_t m = (i < 23) ? i : 23;          /* how many previous terms to sum */
        for (size_t j = 1; j <= m; ++j)
            sum += cache[i - j];

        seq_t val = sum * (seq_t)25;           /* 25·sum */
        if (i == 24)
            val += (seq_t)1;                   /* +1 for n==24 */
        else if (i <= 23)
            val += (seq_t)2;                   /* +2 for 1 ≤ n ≤ 23 */

        if (i >= 24)
            val -= cache[i - 24] * (seq_t)325; /* –325·a(n‑24) for n≥24 */

        cache[i] = val;
    }

    len = (size_t)n + 1;
    return cache[(size_t)n];
}
