/*
 * A202807  Number of n X 3 nonnegative integer arrays with each row and
 *          column increasing from zero by 0, 1 or 2.
 * https://oeis.org/A202807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) * +
 *            21*a(n-5) - 7*a(n-6) + a(n-7) for n > 7 * Initial values:
 *            a(1)=1, a(2)=6, a(3)=32, a(4)=121, a(5)=356, * a(6)=881,
 *            a(7)=1925
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 94, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 210 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;          /* cached sequence values  */
static idx_t cached = 0;             /* number of terms stored   */
static const seq_t init[7] = { 1, 6, 32, 121, 356, 881, 1925 };

/* Return the n-th term (n>=1) of sequence A202807. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n=0 not used */

    /* Initial allocation and seeding with first seven terms. */
    if (cached == 0) {
        size_t sz = 7;
        cache = mem_new(seq_t, sz);
        for (size_t i = 0; i < 7; ++i) cache[i] = init[i];
        cached = 7;
    }

    /* Return cached value if already computed. */
    if (n <= cached) return cache[n - 1];

    /* Expand cache to hold up to n terms. */
    size_t old = cached;
    size_t new_sz = (size_t)n;
    seq_t *tmp = mem_resize(seq_t, cache, new_sz);
    if (!tmp) {
        /* allocation failed; try a fresh allocation (unlikely). */
        tmp = mem_new(seq_t, new_sz);
        if (!tmp) return 0;  /* out of memory – return 0 as sentinel */
        mem_free(cache);
    }
    cache = tmp;

    /* Compute missing terms using the recurrence. */
    for (idx_t i = old + 1; i <= n; ++i) {
        if (i <= 7) continue;  /* already initialized */

        seq_t val = 0;
        val +=  7 * cache[i - 2];
        val -= 21 * cache[i - 3];
        val += 35 * cache[i - 4];
        val -= 35 * cache[i - 5];
        val += 21 * cache[i - 6];
        val -=  7 * cache[i - 7];
        val += cache[i - 8];
        cache[i - 1] = val;
    }

    cached = n;
    return cache[n - 1];
}
