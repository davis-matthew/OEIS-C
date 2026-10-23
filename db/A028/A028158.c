/*
 * A028158  Expansion of 1/((1-4x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A028158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*11^(n+3)-14*10^(n+3)+7*8^(n+3)-4^(n+3))/168
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* number of terms already computed */
    static size_t cap = 0;      /* current capacity of cache array */

    /* Allocate an initial cache if necessary */
    if (!cache) {
        cap = 8;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure sentinel */
    }

    /* Ensure the cache can hold terms up to index n (inclusive) */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t new_cap = cap;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        cache = tmp;
        cap = new_cap;
    }

    /* Compute missing terms using the linear recurrence:
       a(n) = 33*a(n‑1) – 394*a(n‑2) + 1992*a(n‑3) – 3520*a(n‑4)   (n ≥ 4) */
    for (size_t i = cached; i < need; ++i) {
        seq_t val;
        if (i == 0)                val = (seq_t)1;
        else if (i == 1)           val = (seq_t)33;
        else if (i == 2)           val = (seq_t)695;
        else if (i == 3)           val = (seq_t)11925;
        else {
            val = 33 * cache[i-1];
            val -= 394 * cache[i-2];
            val += 1992 * cache[i-3];
            val -= 3520 * cache[i-4];
        }
        cache[i] = val;
    }
    cached = need;
    return cache[(size_t)n];
}
