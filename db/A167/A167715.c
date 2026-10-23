/*
 * A167715  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^15 = I.
 * https://oeis.org/A167715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1..14)=2, a(15)=1, a(n>15)=0; Recurrence: for n≥1 a(n)
 *            = N_n + 28·(a(n-1)+…+a(n-14)) – 406·a(n-15) where N_n = 1 if n=0
 *            or n=15, 2 if 1≤n≤14, 0 otherwise. This follows from the
 *            rational generating function in OEIS
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static idx_t last_computed = 0;    /* highest index cached */

    /* enlarge cache if needed */
    if (n >= capacity) {
        size_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* first call: initialise a(0) */
    if (last_computed == 0 && cache[0] == 0)
        cache[0] = (seq_t)1;

    /* compute missing terms up to n */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        seq_t sum;
        /* N_k */
        if (k == 0 || k == 15)
            sum = (seq_t)1;
        else if (k <= 14)
            sum = (seq_t)2;
        else
            sum = (seq_t)0;

        /* add 28·a(k-j) for j=1..14 */
        for (int j = 1; j <= 14; ++j) {
            if (k >= (idx_t)j)
                sum += (seq_t)28 * cache[k - (idx_t)j];
        }
        /* subtract 406·a(k-15) if applicable */
        if (k >= 15)
            sum -= (seq_t)406 * cache[k - 15];

        cache[k] = sum;
    }
    last_computed = n;
    return cache[n];
}
