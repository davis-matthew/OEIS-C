/*
 * A025030  Number of antichains in the product of a fence poset with n
 *          elements and a chain with 6 elements; also number of paths with n
 *          turns when light is reflected from 7 glass plates.
 * https://oeis.org/A025030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 6*a(n-2) - 10*a(n-3) - 5*a(n-4) + 6*a(n-5) +
 *            a(n-6) - a(n-7); Initial values: a(0)=1, a(1)=7, a(2)=28,
 *            a(3)=140, a(4)=658, a(5)=3164, a(6)=15106
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

static const seq_t init[7] = {
    (seq_t)1, (seq_t)7, (seq_t)28, (seq_t)140,
    (seq_t)658, (seq_t)3164, (seq_t)15106
};

#define MAX_TERMS   12000   /* sufficient for n up to about 10000 */

static seq_t cache[MAX_TERMS];
static size_t computed = 0;   /* number of terms initialized in cache */

/* Ensure the first seven terms are in the cache */
static void init_cache(void) {
    if (computed == 0) {
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        computed = 7;
    }
}

seq_t a(idx_t n) {
    init_cache();
    size_t idx = (size_t)n;
    if (idx >= computed) {
        if (idx >= MAX_TERMS) {
            /* Out of bounds – undefined; the problem guarantees n ≤ ~10000. */
        }
        for (size_t i = computed; i <= idx; ++i) {
            seq_t v1 = cache[i-1];
            seq_t v2 = cache[i-2];
            seq_t v3 = cache[i-3];
            seq_t v4 = cache[i-4];
            seq_t v5 = cache[i-5];
            seq_t v6 = cache[i-6];
            seq_t v7 = cache[i-7];

            seq_t val = 4*v1 + 6*v2;
            val -= 10*v3;
            val -= 5*v4;
            val += 6*v5;
            val += v6;
            val -= v7;

            cache[i] = val;
        }
        computed = idx + 1;
    }
    return cache[idx];
}
