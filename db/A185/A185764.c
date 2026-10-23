/*
 * A185764  Number of (n+1) X 5 binary arrays with no 2 X 2 subblock trace
 *          equal to any horizontal or vertical neighbor 2 X 2 subblock trace.
 * https://oeis.org/A185764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 17*a(n-2) + 9*a(n-3) - 22*a(n-4) - a(n-5) +
 *            a(n-6) - 6*a(n-7) * Initial values: * a(1) = 264 * a(2) = 1128 *
 *            a(3) = 5272 * a(4) = 24832 * a(5) = 118248 * a(6) = 562800 *
 *            a(7) = 2680000 * a(8) = 12760448 * a(9) = 60758008 * This
 *            recurrence is valid for n > 9 (i.e. n ≥ 10). The impl...
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *seq_cache = NULL;
static idx_t cache_cap = 0;

static const seq_t init_vals[10] = {
    (seq_t)0,
    (seq_t)264,
    (seq_t)1128,
    (seq_t)5272,
    (seq_t)24832,
    (seq_t)118248,
    (seq_t)562800,
    (seq_t)2680000,
    (seq_t)12760448,
    (seq_t)60758008
};

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (seq_cache == NULL) {
            seq_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        } else {
            seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        }
        /* Initialize newly allocated tail with zeros */
        if (seq_cache == NULL) return (seq_t)0; /* allocation failed */
        for (idx_t i = cache_cap; i < new_cap; ++i) seq_cache[i] = 0;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (seq_cache[i] != 0) continue;                /* already computed */
        if (i <= 9) {
            seq_cache[i] = init_vals[i];
            continue;
        }
        /* recurrence: a[n] = a[n-1] + 17*a[n-2] + 9*a[n-3] - 22*a[n-4] - a[n-5] + a[n-6] - 6*a[n-7] */
        seq_t t1 = seq_cache[i-1];
        seq_t t2 = (seq_t)17 * seq_cache[i-2];
        seq_t t3 = (seq_t)9  * seq_cache[i-3];
        seq_t t4 = (seq_t)22 * seq_cache[i-4];
        seq_t t5 = seq_cache[i-5];
        seq_t t6 = seq_cache[i-6];
        seq_t t7 = (seq_t)6  * seq_cache[i-7];
        seq_cache[i] = t1 + t2 + t3 - t4 - t5 + t6 - t7;
    }
    return seq_cache[n];
}
