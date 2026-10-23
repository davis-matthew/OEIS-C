/*
 * A028935  a(n) = A006720(n)^3 (cubed terms of Somos-4 sequence).
 * https://oeis.org/A028935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006720(n)^3 (cubes of the Somos‑4 sequence). Initial
 *            terms: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=8. For n≥10: a(n) =
 *            (129·a(n-1)·a(n-8) - 260·a(n-2)·a(n-7) - 8385·a(n-3)·a(n-6) +
 *            48633·a(n-4)·a(n-5)) / a(n-9)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[10] = {
    1, 1, 1, 1,
    8, 27, 343, 12167,
    205379, 30959144
};

static seq_t *cache = NULL;
static idx_t capacity = 0;   /* allocated array size */
static idx_t computed = 0;   /* number of terms stored (0..computed-1) */

seq_t a(idx_t n) {
    /* initialise the cache with the known first 10 terms */
    if (cache == NULL) {
        capacity = computed = 10;
        cache = (seq_t *)malloc(sizeof(seq_t) * capacity);
        for (size_t i = 0; i < 10; ++i) cache[i] = init_vals[i];
    }

    if (n < computed) return cache[(size_t)n];

    /* enlarge capacity if needed */
    size_t needed = (size_t)n + 1;
    if (needed > capacity) {
        size_t new_cap = capacity ? capacity : 1;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        capacity = new_cap;
    }

    /* compute missing terms from 'computed' up to 'n' */
    for (size_t idx = computed; idx < needed; ++idx) {
        seq_t a1 = cache[idx-1];
        seq_t a2 = cache[idx-2];
        seq_t a3 = cache[idx-3];
        seq_t a4 = cache[idx-4];
        seq_t a5 = cache[idx-5];
        seq_t a6 = cache[idx-6];
        seq_t a7 = cache[idx-7];
        seq_t a8 = cache[idx-8];
        seq_t a9 = cache[idx-9];

        seq_t num = (seq_t)129 * a1 * a8;
        num = num - (seq_t)260 * a2 * a7;
        num = num - (seq_t)8385 * a3 * a6;
        num = num + (seq_t)48633 * a4 * a5;
        cache[idx] = num / a9;   /* division is exact by the recurrence */
    }

    computed = needed;
    return cache[(size_t)n];
}
