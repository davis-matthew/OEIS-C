/*
 * A167953  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^16 = I.
 * https://oeis.org/A167953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(1) = 37 a(n) = 36·a(n‑1) for 2 ≤ n ≤ 15 a(16) =
 *            36·a(15) – 666 a(17) = 36·a(16) – 665·a(1) + 630·a(0) – 1 a(n) =
 *            36·a(n‑1) – 665·a(n‑16) + 630·a(n‑17) for n ≥ 18 (Obtained from
 *            the generating function (1+t)(1‑t^16)/(1‑36t+665t^16‑630t^17).)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* dynamic array of a(n)      */
    static size_t cached_size = 0;   /* number of values stored    */
    static size_t cached_cap  = 0;   /* capacity of the array      */

    /* Ensure adequete capacity */
    if ((idx_t)(cached_cap) <= n) {
        size_t new_cap = cached_cap ? cached_cap : 16;
        while ((idx_t)(new_cap) <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cached_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cached_size; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = (seq_t)1;
        } else if (i == 1) {
            val = (seq_t)37;
        } else if (i <= 15) {                /* 2 ≤ i ≤ 15  */
            val = (seq_t)36 * cache[i-1];
        } else if (i == 16) {                /* special n = 16 */
            val = (seq_t)36 * cache[15] - (seq_t)666;
        } else if (i == 17) {                /* special n = 17 */
            val = (seq_t)36 * cache[16]
                - (seq_t)665 * cache[1]
                + (seq_t)630 * cache[0]
                - (seq_t)1;
        } else {                              /* n ≥ 18  */
            val = (seq_t)36 * cache[i-1]
                - (seq_t)665 * cache[i-16]
                + (seq_t)630 * cache[i-17];
        }
        cache[i] = val;
    }

    cached_size = (size_t)(n + 1);
    return cache[n];
}
