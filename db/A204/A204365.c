/*
 * A204365  Number of (n+2) X 5 0..2 arrays with every 3 X 3 subblock having
 *          three equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally exactly three ways, and new values 0..2 introduced in
 *          row major order.
 * https://oeis.org/A204365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-4) + 4*a(n-6) for n>9; Initial values:
 *            a(1)=555, a(2)=673, a(3)=342, a(4)=622, a(5)=1042, a(6)=2574,
 *            a(7)=6786, a(8)=17586, a(9)=47674
 * Range:     u8 none, u16 n <= 9, u32 n <= 20, u64 n <= 42, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base[10] = {
    (seq_t)0,   (seq_t)555,  (seq_t)673,  (seq_t)342,
    (seq_t)622, (seq_t)1042, (seq_t)2574, (seq_t)6786,
    (seq_t)17586, (seq_t)47674
};

/* memoization buffer (index 0 unused) */
static seq_t *cache   = NULL;    /* pointer to buffer */
static idx_t  cache_n = 0;       /* highest index allocated */

/* maximal n computed so far (value stored in cache[1..cache_n]) */
static idx_t  last_computed = 0;

seq_t a(idx_t n)
{
    /* offset is 1 */
    if (n == 0) return (seq_t)0;          /* out of range, return 0 */
    if (n <= 9) return base[n];           /* known initial values */

    /* extend cache if necessary */
    if (n > cache_n) {
        size_t new_size = (size_t)(n + 1); /* index 0..n inclusive */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        cache    = tmp;
        cache_n  = new_size - 1;          /* now allocated up to n */

        /* ensure base values are present */
        if (last_computed < 9) {
            for (idx_t i = 1; i <= 9; ++i) cache[i] = base[i];
            last_computed = 9;
        }

        /* compute values from max(last_computed+1,10) up to n */
        idx_t start = last_computed + 1;
        if (start < 10) start = 10;
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 3 * cache[i-1] - 6 * cache[i-4] + 4 * cache[i-6];
        }
        last_computed = n;
    }

    return cache[n];
}
