/*
 * A189651  Number of 3Xn array permutations with each element moving zero or
 *          one space horizontally, diagonally or antidiagonally.
 * https://oeis.org/A189651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) +75*a(n-2) -240*a(n-3) -399*a(n-4) +972*a(n-5)
 *            * - 1263*a(n-6) + 482*a(n-7) + 669*a(n-8) -100*a(n-9) -9*a(n-10)
 *            * Initial values (n = 1..10): * a(1)=1, a(2)=32, a(3)=263,
 *            a(4)=4277, a(5)=55440, * a(6)=799069, a(7)=11047585,
 *            a(8)=155610272, * a(9)=2174615543, a(10)=30495310793
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

static const int coeff[10] = { 10, 75, -240, -399, 972, -1263, 482, 669, -100, -9 };
static const seq_t init_vals[10] = {
    1ULL, 32ULL, 263ULL, 4277ULL, 55440ULL,
    799069ULL, 11047585ULL, 155610272ULL, 2174615543ULL, 30495310793ULL
};

seq_t a(idx_t n)
{
    /* Offset is 1: a(1) is the first term.  For n==0 undefined; return 0. */
    if (n == 0) return 0;

    /* Static cache to memoize results.  The cache indices start at 1. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* capacity of cache (indices 0..cache_size-1) */
    static size_t computed = 0;     /* largest n that has been computed */

    /* Ensure cache is big enough for index n. */
    if (n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise initial values if they haven't been set yet. */
    if (computed < 10) {
        size_t start = computed + 1;
        if (start < 1) start = 1;
        for (size_t i = start; i <= 10 && i <= (size_t)n; ++i) {
            cache[i] = init_vals[i - 1];
        }
        if (computed < 10) computed = 10;
    }

    /* Compute values for indices > computed up to n. */
    for (size_t idx = computed + 1; idx <= (size_t)n; ++idx) {
        if (idx <= 10) {  /* already initialised */
            continue;
        }
        seq_t sum = 0;
        for (int k = 0; k < 10; ++k) {
            seq_t term = cache[idx - (k + 1)];
            int c = coeff[k];
            if (c >= 0) {
                sum += mul_small(term, (unsigned)c);
            } else {
                sum -= mul_small(term, (unsigned)(-c));
            }
        }
        cache[idx] = sum;
    }

    computed = (size_t)n;
    return cache[n];
}
