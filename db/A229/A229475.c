/*
 * A229475  Number of defective 4-colorings of an n X 4 0..3 array connected
 *          horizontally, antidiagonally and vertically with exactly one
 *          mistake, and colors introduced in row-major 0..3 order.
 * https://oeis.org/A229475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 24*a(n-1)-224*a(n-2)+1096*a(n-3)-3360*a(n-4) *
 *            +6976*a(n-5)-9744*a(n-6)+8704*a(n-7)-4096*a(n-8) for n>9 *
 *            Initial values: * a(1)=6, a(2)=248, a(3)=3328, a(4)=36116,
 *            a(5)=360912, * a(6)=3443856, a(7)=31875248, a(8)=288634368,
 *            a(9)=2570642816
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* forward declaration */
seq_t a(idx_t n);

/* static cache of computed values */
static seq_t *cache = NULL;       /* array of computed a(i) values   */
static size_t cache_cap = 0;      /* current allocated size (entries) */

/* initial values a(0)..a(9) (a(0) unused) */
static const seq_t init_vals[10] = {
    0ULL,                /* dummy for index 0 */
    6ULL,                /* a(1) */
    248ULL,              /* a(2) */
    3328ULL,             /* a(3) */
    36116ULL,            /* a(4) */
    360912ULL,           /* a(5) */
    3443856ULL,          /* a(6) */
    31875248ULL,         /* a(7) */
    288634368ULL,        /* a(8) */
    2570642816ULL       /* a(9) */
};

/* ensure that cache contains values up to and including index n (inclusive) */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;

    size_t needed_cap = (size_t)n + 1;       /* indices 0..n inclusive */

    if (needed_cap <= cache_cap)
        return;                              /* already sufficient */

    /* reallocate to the required size */
    seq_t *new_cache = (seq_t *)realloc(cache, needed_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                            /* out of memory */

    cache = new_cache;
    size_t old_cap = cache_cap;
    cache_cap = needed_cap;

    /* fill any newly allocated entries */
    for (size_t i = old_cap; i < cache_cap; ++i)
    {
        if (i <= 9)
        {
            cache[i] = init_vals[i];
        }
        else
        {
            /* recurrence for n > 9 */
            seq_t plus  = 24u   * cache[i - 1]
                        + 1096u * cache[i - 3]
                        + 6976u * cache[i - 5]
                        + 8704u * cache[i - 7];

            seq_t minus = 224u   * cache[i - 2]
                        + 3360u * cache[i - 4]
                        + 9744u * cache[i - 6]
                        + 4096u * cache[i - 8];

            cache[i] = plus - minus;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* sequence defined for n >= 1 only */

    ensure_cache(n);
    return cache[n];
}
