/*
 * A169418  Number of reduced words of length n in Coxeter group on 21
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0)=1 for n≥1: a(n) =
 *            n_N(n) + 19⋅∑_{k=1}^{31} a(n−k) − 190⋅a(n−32) where n_N(0)=1,
 *            n_N(k)=2 for 1≤k≤31, n_N(32)=1 and n_N(k)=0 otherwise. This
 *            recurrence follows from the generating function
 *            (1+2∑_{k=1}^{31}x^k+x^32)/(1−19∑_{k=1}^{31}x^k+190x^32)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t size = 0;

    if (n < size) return cache[n];

    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);

    for (idx_t i = size; i <= n; ++i) {
        seq_t ncoeff;
        if (i == 0) ncoeff = (seq_t)1;
        else if (i <= 31) ncoeff = (seq_t)2;
        else if (i == 32) ncoeff = (seq_t)1;
        else ncoeff = (seq_t)0;

        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= 31 && i >= j; ++j) {
            sum += new_cache[(size_t)(i - j)];
        }

        seq_t term = ncoeff + 19 * sum;
        if (i >= 32) term -= 190 * new_cache[(size_t)(i - 32)];
        new_cache[(size_t)i] = term;
    }

    cache = new_cache;
    size = new_size;
    return cache[(size_t)n];
}
