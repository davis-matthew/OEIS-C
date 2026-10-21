/*
 * A224033  Number of n X 3 0..1 arrays with antidiagonals unimodal and rows
 *          and diagonals nondecreasing.
 * https://oeis.org/A224033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3) + 2*a(n-5); a(1)=4, a(2)=13,
 *            a(3)=37, a(4)=105, a(5)=298;
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

extern seq_t a(idx_t n);

#define MAX_CACHE_N 20000     /* sufficient for n up to about 10000 */

static seq_t cache[MAX_CACHE_N + 1];
static idx_t cache_max = 0;

/* stores the first five terms once */
static void init_base(void)
{
    static const seq_t base[5] = {4, 13, 37, 105, 298};
    for (idx_t i = 1; i <= 5; ++i)
        cache[i] = base[i - 1];
    cache_max = 5;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is 1‑based; 0 is undefined */

    /* initialise cache lazily */
    if (cache_max == 0)
        init_base();

    /* already computed? */
    if (n <= cache_max)
        return cache[n];

    /* extend cache up to the requested n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term = 4 * cache[i - 1];
        term -= 3 * cache[i - 2];
        term -= cache[i - 3];
        term += 2 * cache[i - 5];
        cache[i] = term;
    }
    cache_max = n;
    return cache[n];
}
