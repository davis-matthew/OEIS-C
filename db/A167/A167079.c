/*
 * A167079  Number of reduced words of length n in Coxeter group on 26
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A167079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*(a(n-1)+a(n-2)+...+a(n-12)) - 300*a(n-13) + p(n),
 *            where p(n)=1 if n==13, p(n)=0 otherwise. Offset 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Pre‑computed initial terms a(0) … a(12). */
static const seq_t init[13] = {
    (seq_t)1ULL,
    (seq_t)26ULL,
    (seq_t)650ULL,
    (seq_t)16250ULL,
    (seq_t)406250ULL,
    (seq_t)10156250ULL,
    (seq_t)253906250ULL,
    (seq_t)6347656250ULL,
    (seq_t)158691406250ULL,
    (seq_t)3967285156250ULL,
    (seq_t)99182128906250ULL,
    (seq_t)2479553222656250ULL,
    (seq_t)61988830566406250ULL
};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    /* Initialize cache with the known initial values. */
    if (!cache) {
        cap = 13;
        size = 13;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < cap; ++i) cache[i] = init[i];
    }

    size_t idx = (size_t)n;

    /* Resize cache if needed. */
    if (idx >= cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= idx) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to idx. */
    if (idx >= size) {
        for (size_t i = size; i <= idx; ++i) {
            seq_t sum = 0;
            for (size_t j = 1; j <= 12; ++j) sum += cache[i - j];
            seq_t val = (seq_t)24 * sum - (seq_t)300 * cache[i - 13];
            if (i == 13) val += (seq_t)1;   /* numerator coefficient at n=13 */
            cache[i] = val;
        }
        size = idx + 1;
    }

    return cache[idx];
}
