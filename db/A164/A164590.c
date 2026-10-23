/*
 * A164590  Number of reduced words of length n in Coxeter group on 11
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -45*a(n-7) + 9*Sum_{k=1..6} a(n-k), with initial values
 *            a(0)=1, a(1)=11, a(2)=110, a(3)=1100, a(4)=11000, a(5)=110000,
 *            a(6)=1100000. The recurrence includes a constant term +1 only
 *            for n = 7, coming from the numerator of the generating function
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;      /* allocated capacity */
    static size_t computed = 0; /* number of values stored (>=7 after init) */

    /* Initial terms as given */
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)110,
        (seq_t)1100,
        (seq_t)11000,
        (seq_t)110000,
        (seq_t)1100000
    };

    /* Ensure space for indices 0 … n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (cap > 0 ? cap : 16);
        while ((size_t)n + 1 > newcap) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed */
        for (size_t i = cap; i < newcap; ++i) tmp[i] = (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* First‑time initialization of cache */
    if (computed == 0) {
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        computed = 7;
    }

    /* Compute missing values up to n (inclusive) */
    if (n >= 7) {
        size_t start = (computed < 7 ? 7 : computed);
        for (size_t i = start; i <= (size_t)n; ++i) {
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 6; ++k) sum += cache[i - k];
            seq_t val = (seq_t)9 * sum - (seq_t)45 * cache[i - 7];
            if (i == 7) val += (seq_t)1; /* constant term from numerator */
            cache[i] = val;
        }
        computed = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
