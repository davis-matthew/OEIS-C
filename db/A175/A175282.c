/*
 * A175282  Positive numbers n with property that n^2+3n+9 is prime (A005471).
 * https://oeis.org/A175282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   positive integers n such that n^2 + 3n + 9 is prime (OEIS
 *            A175282)
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* sequence cache */
    static size_t cap = 0;          /* allocated capacity */
    static idx_t found = 0;         /* number of terms stored */
    static idx_t current_n = 1;     /* next candidate n to test */

    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!cache) abort();
    }

    while (found < n) {
        seq_t cand = (seq_t)current_n;
        seq_t val  = cand * cand + (seq_t)3 * cand + (seq_t)9;

        if (is_prime_big(val)) {
            if ((size_t)found >= cap) {
                cap *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
            }
            cache[found++] = cand;
        }
        current_n++;
    }

    return cache[n - 1];
}
